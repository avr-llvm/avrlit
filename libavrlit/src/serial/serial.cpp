#include "serial.h"

using namespace avrlit;

namespace {
  SerialImpl *CurrentSerial;
}

void SerialImpl::SetCurrent(SerialImpl *serial) {
  CurrentSerial = serial;
}

SerialImpl *SerialImpl::GetCurrent() {
  return CurrentSerial;
}

static Serial serial;

void SerialImpl::Initialize() {
  SetCurrent(&serial);
}
