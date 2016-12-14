# AVR LLVM Integrated Tester

This tool builds an AVR executable from test code lowered by the LLVM backend and the
`libavrlit` test suite library using a known good toolchain (avr-gcc). The
resulting binary is uploaded to a development board. Test results are collected
using a virtual tty.

### Description

For any testcase, we run a custom LLVM instrumentation pass. This pass lives inside
LLVM itself, in `lib/Target/AVR/AVRInstrumentFunctions.cpp`. When this pass is run,
it inserts calls to external hook functions directly into the executable.

When the executable is linked, you must then also link object files containing
definitions for the hook functions.

We have a support library in this repository named `libavrlit`. This library
implements the instrumented hook functions and sending all of the information
through a serial port.

The host computer can then link tests to the `libavrlit` library, run them,
and then inspect the results.

### Setup

There are two ways to use this tool - from within LLVM as a testing tool, or
standalone.

Things you will need:
  * LLVM
  * ATmega328p board (Arduino Uno)
  * ~ATmega32U4 board with USB and AVR-109 type bootloader. (Arduino Leonardo,
    RedBear Blend (Micro), &c.)~ currently broken
  * [`pySerial`](http://pyserial.sourceforge.net) python module
  * avr-gcc (for compiling the support library)
  * avrdude (for flashing tests to the chips)
 llvm_paths["binaries"],  * Fire extinguisher

Before tests are run, you need to tell AVRLIT about your board

```bash
# Tell AVRLIT about the board.
export AVRLIT_BOARD=uno
export AVRLIT_PORT=/dev/cu.usbserial-A9OFZL9T
```

### Running standalone

```bash
# Compile and upload a test, print only the test output to standard out.
avrlit.py.in ~/tests/avr/test.ll

# Verbose mode
avrlit.py.in ~/tests/avr/test.ll -v
```

### Running from within LLVM

```bash
# Grab LLVM
git clone https://github.com/llvm-mirror/llvm.git

# Grab avrlit and put it inside 'llvm/tools'
cd llvm/tools
git clone https://github.com/avr-llvm/avrlit.git

cd ../../

# Build LLVM
mkdir llvm-build && cd llvm-build
cmake ../llvm -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=AVR

# Run all LLVM tests, including the AVR metal tests
make check

# OR alternatively, run the metal tests specifically
./bin/llvm-lit ../llvm/test/Metal/AVR
```

### Writing Tests

The on-target execution tests reside in `llvm/test/Metal/AVR`. Like other lit
tests they contain a `RUN: ` line calling `avrlit`:

```llvm
; RUN: avrlit %s | FileCheck %s

; CHECK-LABEL: test
define i16 @test() {
  ; CHECK-NEXT: return 1357
  ret i16 1357
}
```
