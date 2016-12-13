#include "avrlit.h"

#include <stdlib.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <util/delay.h>

#include <LUFA/Drivers/USB/USB.h>

#include "usb.h"

namespace avrlit {

struct USB_ClassInfo_CDC_Device_t : ::USB_ClassInfo_CDC_Device_t {};

USBSerial::USBSerial(USBSerial::USB_ClassInfo_CDC_Device_t *cdc) : cdc_(cdc) {};

// USBSerial get_cdc_stream() { return USBSerial((USBSerial::USB_ClassInfo_CDC_Device_t*)&cdc); }

// uint8_t USBSerial::receive_byte() {
//   return CDC_Device_ReceiveByte(static_cast<USB_ClassInfo_CDC_Device_t*>(cdc_));
// }
//
// void USBSerial::send(uint8_t value) {
//   CDC_Device_SendByte(cdc_, value);
// }

bool is_terminal_ready = false;

void
init() {
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  clock_prescale_set(clock_div_1);

  USB_Init();

  sei();
}

// void
// do_tasks(bool performRead) {
//   if (performRead) CDC_Device_ReceiveByte(&cdc);
//   CDC_Device_USBTask(&cdc);
//   USB_USBTask();
// }

inline
void
process_control_line_change(USB_ClassInfo_CDC_Device_t *const device) {
  // this implements the same reset behaviour as the Arduino Leonardo
  uint16_t control_lines = device->State.ControlLineStates.HostToDevice;
  is_terminal_ready = control_lines & CDC_CONTROL_LINE_OUT_DTR;
  if (not is_terminal_ready and
      device->State.LineEncoding.BaudRateBPS == 1200)
  {
    USB_Disable();
    cli();

    *(uint16_t *)0x0800 = 0x7777;
    wdt_enable(WDTO_120MS);
  } else {
    wdt_disable();
    wdt_reset();
    *(uint16_t *)0x0800 = 0x0;
  }
}

} // end of namespace avrlit

//=== USB Event Hooks =========================================================

void
EVENT_USB_Device_ConfigurationChanged() {
  CDC_Device_ConfigureEndpoints(&cdc);
}

void
EVENT_USB_Device_ControlRequest() {
  CDC_Device_ProcessControlRequest(&cdc);
}

void
EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t *const d) {
  avrlit::process_control_line_change(d);
}

