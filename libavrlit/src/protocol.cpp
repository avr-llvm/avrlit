//===-- avrlit - AVR LLVM Integrated Tester - AVR Side --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "protocol.h"

using namespace avrlit;

Value Value::Read(ostream &stream) {
  TypeId typeId = (TypeId)stream.read_byte();

  Value value { typeId };

  switch (typeId) {
  case TypeI8:
    value.I8.value = stream.read_byte();
    break;
  case TypeI16: {
    value.I16.value = stream.read_u16();
    break;
  }
  case TypeI32: {
    value.I32.value = stream.read_u32();
    break;
  }
  case TypeI64: {
    value.I64.value = stream.read_u64();
  }
  default: {
    // abort
  }
  }

  return value;
}
