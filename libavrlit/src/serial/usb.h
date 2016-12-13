#pragma once

#include <stdint.h>

#include "serial.h"

namespace avrlit {

class USBSerial : public SerialImpl {
  public:
    struct USB_ClassInfo_CDC_Device_t;
    explicit USBSerial(USB_ClassInfo_CDC_Device_t * cdc);

    void send(uint8_t byte) override;
    uint8_t receive_byte() override;
  private:
    USB_ClassInfo_CDC_Device_t * cdc_;
};

} // end of namespace avrlit
