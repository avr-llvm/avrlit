//===-- avrlit - AVR LLVM Integrated Tester - AVR Side --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "avrlit.h"

#include <stdint.h>

namespace avrlit
{

//! A number identifying a type.
enum TypeId {
  TypeI8 = 0,
  TypeI16 = 1,
  TypeI32 = 2,
  TypeI64 = 3,
};

//! A type that can be sent over the wire.
struct Type
{
  TypeId id;

  union { uint8_t value; } I8;
  union { uint16_t value; } I16;
  union { uint32_t value; } I32;
  union { uint64_t value; } I64;

  Type Read(avrlit::ostream &stream);
};

}
