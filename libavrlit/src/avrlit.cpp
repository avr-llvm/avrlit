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
void reportInteger(const char *name, T result, bool includeColon) {
  avrlit::SerialImpl::GetCurrent()->send(name);

  if (includeColon)
    avrlit::SerialImpl::GetCurrent()->send(": ");

  char buffer[20];
  ltoa(result, buffer, 10);

  avrlit::SerialImpl::GetCurrent()->send(buffer);
}

void reportFloat(const char *name, float f, bool includeColon) {
  avrlit::SerialImpl::GetCurrent()->send(name);

  if (includeColon)
    avrlit::SerialImpl::GetCurrent()->send(": ");

  char buffer[40];
  sprintf(buffer, "%f", (double)f);

  avrlit::SerialImpl::GetCurrent()->send(buffer);
}

template<typename T>
void reportArgumentInteger(const char *name, uint8_t idx, T result) {
  if (idx > 0) {
    avrlit::SerialImpl::GetCurrent()->send(", ");
  }

  reportInteger(name, result, true);
}
void reportArgumentFloat(const char *name, uint8_t idx, float result) {
  if (idx > 0) {
    avrlit::SerialImpl::GetCurrent()->send(", ");
  }

  reportFloat(name, result, true);
}

template<typename T>
void reportResultInteger(T result) {
  reportInteger("return ", result, false);
  avrlit::SerialImpl::GetCurrent()->send('\n');
}

void reportResultFloat(float result) {
  reportFloat("return ", result, false);
  avrlit::SerialImpl::GetCurrent()->send('\n');
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

  void avr_instrumentation_argument_i8(const char *argName, uint8_t idx, uint8_t val) { reportArgumentInteger(argName, idx, val); }
  void avr_instrumentation_argument_i16(const char *argName, uint8_t idx, uint16_t val) { reportArgumentInteger(argName, idx, val); }
  void avr_instrumentation_argument_i32(const char *argName, uint8_t idx, uint32_t val) { reportArgumentInteger(argName, idx, val); }
  void avr_instrumentation_argument_i64(const char *argName, uint8_t idx, uint64_t val) { reportArgumentInteger(argName, idx, val); }

  void avr_instrumentation_argument_f32(const char *argName, uint8_t idx, float val) { reportArgumentFloat(argName, idx, val); }

  void avr_instrumentation_result_i8(uint8_t result) { reportResultInteger(result); }
  void avr_instrumentation_result_i16(uint16_t result) { reportResultInteger(result); }
  void avr_instrumentation_result_i32(uint32_t result) { reportResultInteger(result); }
  void avr_instrumentation_result_i64(uint64_t result) { reportResultInteger(result); }

  void avr_instrumentation_result_f32(float result) { reportResultFloat(result); }
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
