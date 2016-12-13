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

extern "C" {
  void InitializeAVRLit() {
    avrlit::SerialImpl::Initialize();
  }
}

int main() {
  InitializeAVRLit();

  avrlit::SerialImpl::GetCurrent()->send("hello world\n");
}
