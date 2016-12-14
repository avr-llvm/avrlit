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

namespace {

template<typename T>
void reportInteger(const char *name, T result) {
  avrlit::SerialImpl::GetCurrent()->send(name);
  avrlit::SerialImpl::GetCurrent()->send(": ");

  char buffer[20];
  ltoa(result, buffer, 10);

  avrlit::SerialImpl::GetCurrent()->send(buffer);
}

}

extern "C" {
  void avrlit_initialize() {
    avrlit::SerialImpl::Initialize();
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

    if (argCount == 0) {
      avrlit::SerialImpl::GetCurrent()->send("()\n");
    } else {
      avrlit::SerialImpl::GetCurrent()->send("(\n");
    }
  }

  void avr_instrumentation_end_signature(const char *funcName, uint16_t argCount) {
    if (argCount != 0) {
      avrlit::SerialImpl::GetCurrent()->send(")\n");
    }
  }

  void avr_instrumentation_result_u16(uint16_t result) {
    reportInteger("return", result);
  }
}

extern "C" {
  // The testing entry point.
  void test();
}

int main() {
  avrlit_initialize();
  test();
}
