//===-- avrlit - AVR LLVM Integrated Tester - AVR Side --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "avrlit.h"
#include "serial/serial.h"

#include <stdlib.h>
#include <stdio.h>

namespace {


template<typename T>
void print(T value);

template<typename T>
void printInteger(T val) {
  char buffer[20];
  ltoa(val, buffer, 10);

  avrlit::SerialImpl::GetCurrent()->send(buffer);
}

template<> void print(uint8_t val) { printInteger(val); }
template<> void print(uint16_t val) { printInteger(val); }
template<> void print(uint32_t val) { printInteger(val); }
template<> void print(uint64_t val) { printInteger(val); }

template<>
void print(float val) {
  char buffer[40];
  sprintf(buffer, "%f", (double)val);

  avrlit::SerialImpl::GetCurrent()->send(buffer);
}

template<typename T>
void reportArgument(const char *name, uint8_t idx, T result) {
  if (idx > 0)
    avrlit::SerialImpl::GetCurrent()->send(", ");

  avrlit::SerialImpl::GetCurrent()->send(name);
  avrlit::SerialImpl::GetCurrent()->send(": ");

  print(result);
}

template<typename T>
void reportResult(T result) {
  avrlit::SerialImpl::GetCurrent()->send("return ");
  print(result);
  avrlit::SerialImpl::GetCurrent()->send('\n');
}

template<typename A, typename B>
void reportAssertionError(A a, B b) {
  avrlit::SerialImpl::GetCurrent()->send("assertion error: ");

  avrlit::SerialImpl::GetCurrent()->send("'");
  print(a);
  avrlit::SerialImpl::GetCurrent()->send("' != '");
  print(b);
  avrlit::SerialImpl::GetCurrent()->send("'\n");

  asm("cli");
  while(1) { }
}
}

extern "C" {
  void avrlit_start() {
    avrlit::SerialImpl::Initialize();
  }

  void avrlit_end() {
    avrlit::SerialImpl::GetCurrent()->send((uint8_t)'\0');
  }

  void avrlit_send_byte(uint8_t byte) {
    avrlit::SerialImpl::GetCurrent()->send(byte);
  }

  uint8_t avrlit_receive_byte() {
    return avrlit::SerialImpl::GetCurrent()->receive_byte();
  }

  /// Hooks that are executed by the test program to report results.
  ///
  /// The LLVM 'avr-instrument-functions' pass does this automatically.

  void avr_instrumentation_begin_signature(const char *funcName, uint16_t argCount) {
    avrlit::SerialImpl::GetCurrent()->send(funcName);

    avrlit::SerialImpl::GetCurrent()->send("(");
  }

  void avr_instrumentation_end_signature(const char *funcName, uint16_t argCount) {
    avrlit::SerialImpl::GetCurrent()->send(")\n");
  }

  void avr_instrumentation_argument_i8(const char *argName, uint8_t idx, uint8_t val) { reportArgument(argName, idx, val); }
  void avr_instrumentation_argument_i16(const char *argName, uint8_t idx, uint16_t val) { reportArgument(argName, idx, val); }
  void avr_instrumentation_argument_i32(const char *argName, uint8_t idx, uint32_t val) { reportArgument(argName, idx, val); }
  void avr_instrumentation_argument_i64(const char *argName, uint8_t idx, uint64_t val) { reportArgument(argName, idx, val); }
  void avr_instrumentation_argument_f32(const char *argName, uint8_t idx, float val) { reportArgument(argName, idx, val); }

  void avr_instrumentation_result_i8(uint8_t result) { reportResult(result); }
  void avr_instrumentation_result_i16(uint16_t result) { reportResult(result); }
  void avr_instrumentation_result_i32(uint32_t result) { reportResult(result); }
  void avr_instrumentation_result_i64(uint64_t result) { reportResult(result); }
  void avr_instrumentation_result_f32(float result) { reportResult(result); }
}

extern "C" {
  // The testing entry point.
  void test();
}

int main() {
  avrlit_start();
  test();
  avrlit_end();

  return 0;
}
