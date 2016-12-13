#pragma once

#include <stdint.h>

#include "serial.h"

namespace avrlit {

class HardwareSerial : public SerialImpl {
  public:
    explicit HardwareSerial();

    void send(uint8_t byte) override;
    uint8_t receive_byte() override;
};

} // end of namespace avrlit
