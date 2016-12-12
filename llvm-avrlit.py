#!/usr/bin/env python

#===-- llvm-avrlit - AVR LLVM Integrated Tester - Host Side ---------------===#
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===-----------------------------------------------------------------------===#

POLL_DELAY = 0.1
MAX_RETRIES = 100

import sys, os, textwrap, subprocess, time, serial
from optparse import OptionParser, OptionGroup

# Variables configured at build time.
llvm_root = "@LLVM_SOURCE_DIR@"
avrlit_dir = os.path.join(llvm_root, "utils", "AVR", "avrlit", "libavrlit")

parser = OptionParser("usage: %prog [options] {ll-and-cpp-files}...")

class Config:
  board = ''
  mcu = ''
  port = ''
  llc = 'llc'
  ld = "avr-gcc"
  output_dir = "/tmp/avrlit"

#=== Build Test Executable ------------------------------------------------===#

def runCommand(executable, arguments):
  print("{} {}".format(executable, " ".join(arguments)))
  subprocess.check_call([executable] + arguments)

# Build the AVRLIT support library.
def buildAvrLit():
  runCommand("make", ["-C", "libavrlit"])

# Compiles an LLVM IR file and returns the object file path.
# Returns the path of the resultant object file.
def compileIR(inputIR, config):
  objectPath = "{}/{}.o".format(config.output_dir, os.path.basename(inputIR))
  arguments = ["-mtriple=avr-atmel-none", "-mcpu", config.mcu,
              "-filetype=obj", inputIR, "-o", objectPath]

  runCommand(config.llc, arguments)
  return objectPath

# Links a set of objects together into an ELF executable.
# Returns the path of the resultant executable file.
def link(objects, config):
  executablePath = "{}/{}.elf".format(config.output_dir, "executable")
  arguments = objects + ["-mmcu=" + config.mcu,
                        "-o", executablePath,
                        "libavrlit/{}/libavrlit.a".format(config.board)]
  runCommand(config.ld, arguments)
  return executablePath

# Generates a Hex file from an executable.
# Returns the path of the resultant hex file.
def generateHex(executable, config):
  hexPath = "{}/{}.hex".format(config.output_dir, os.path.basename(executable))
  arguments = ["-j", ".text", "-j", ".data", "-O", "ihex", executable, hexPath]
  runCommand("avr-objcopy", arguments)
  return hexPath

# Builds a test executable.
# Returns the path of the generated hex file.
def buildTestExecutable(inputIR, config):
  objectPath = compileIR(inputIR, config)
  executablePath = link([objectPath], config)
  hexPath = generateHex(executablePath, config)

  return hexPath

def waitWhilePortExists(port):
  retries = 0
  while os.path.exists(port):
    time.sleep(POLL_DELAY)
    retries += 1
    if retries == MAX_RETRIES:
      raise Exception('Device \'{}\' did not go offline within {} seconds.'
                      . format(port, retries * POLL_DELAY))

def waitUntilPortExists(port):
  retries = 0
  while not os.path.exists(port):
    time.sleep(POLL_DELAY)
    retries += 1
    if retries == MAX_RETRIES:
      raise Exception('Device \'{}\' is not available after {} seconds.'
                      . format(port, retries * POLL_DELAY))

#=== Leonardo -------------------------------------------------------------===#

def resetLeonardo(config):
  serial.Serial(config.port, 1200).close()
  waitWhilePortExists(config.port)

def uploadLeonardo(executable, config):
  resetLeonardo(config)
  waitUntilPortExists(config.port)
  runCommand("avrdude", ['-patmega32u4', '-cavr109',
                         '-P', config.port, '-b57600', '-q', '-q',
                         '-D', '-Uflash:w:{}:i'.format(executable)])
  waitWhilePortExists(port)

#=== Run Executable -------------------------------------------------------===#

def runExecutable(executable, config):
  uploadLeonardo(executable, config)
  waitUntilPortExists(config.port)
  retries = 0
  while True:
    try:
      test = serial.Serial(config.port, 57600, timeout=1)
      break
    except OSError as e:
      time.sleep(POLL_DELAY)
      retries += 1
      if retries == 5:
        raise

  done = False
  passed = True
  timeouts = 0
  while not done:
    line = test.readline().strip()
    if not line:
      timeouts += 1
      if timeouts == 2:
        print("FAIL: Test hangs.")
        passed = False
        done = True
      continue
    # print(line)
    # if line.startswith(b'FAIL:'):
    #   passed = False
    # if line == '--':
    #   done = True
  test.close()
  return passed

#=== Main -----------------------------------------------------------------===#

(opts, files) = parser.parse_args()
if not 'AVRLIT_PORT' in os.environ:
  print('AVRLIT_PORT environment variable is not set')
  exit(1)

port = os.environ['AVRLIT_PORT']
if not os.path.exists(port):
  print('AVRLIT_PORT \'{}\' does not exist'.format(port))
  exit(1)

config = Config()
config.port = port
config.board = os.environ['AVRLIT_BOARD'] if 'AVRLIT_BOARD' in os.environ else 'leonardo'
config.mcu = 'atmega32u4'
config.llc = "/Users/dylan/projects/builds/llvm/bin/llc"
config.output_dir = "/tmp/avrlit"

# Ensure the output directory exists.
if not os.path.exists(config.output_dir):
    os.makedirs(config.output_dir)

buildAvrLit()

for file in files:
  executable = buildTestExecutable(file, config)
  runExecutable(executable, config)
