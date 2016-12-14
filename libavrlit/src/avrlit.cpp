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

  // Unsigned integer reporting
  void avrlit_report_result_u8(const char *name, uint8_t result) { reportInteger(name, result); }
  void avrlit_report_result_u16(const char *name, uint16_t result) { reportInteger(name, result); }
  void avrlit_report_result_u32(const char *name, uint32_t result) { reportInteger(name, result); }
  void avrlit_report_result_u64(const char *name, uint64_t result) { reportInteger(name, result); }

  // Signed integer reporting
  void avrlit_report_result_i8(const char *name, int8_t result) { reportInteger(name, result); }
  void avrlit_report_result_i16(const char *name, int16_t result) { reportInteger(name, result); }
  void avrlit_report_result_i32(const char *name, int32_t result) { reportInteger(name, result); }
  void avrlit_report_result_i64(const char *name, int64_t result) { reportInteger(name, result); }
}
