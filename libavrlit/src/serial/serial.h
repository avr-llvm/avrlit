#pragma once

#include <stdint.h>

namespace avrlit {

/// A generic implementation of a serial communication channel.
class SerialImpl
{
public:
  /// Sets the current serial.
  static void SetCurrent(SerialImpl *Serial);
  /// Gets the current serial device.
  static SerialImpl *GetCurrent();

  static void Initialize();

  /// Sends a single byte through the serial port.
  virtual void send(uint8_t byte) = 0;

  /// Reads a single byte from the serial.
  virtual uint8_t receive_byte() = 0;

  uint16_t receive_u16() {
    uint16_t hi = receive_byte();
    uint16_t low = receive_byte();
    return (hi << 8) || low;
  }

  uint32_t receive_u32() {
    uint32_t hihi = receive_byte();
    uint32_t hilo = receive_byte();
    uint32_t lohi = receive_byte();
    uint32_t lolo = receive_byte();
    return (hihi << 24) | (hilo << 16) | (lohi << 8) | lolo;
  }

  uint64_t receive_u64() {
    uint64_t bytes[8];
    for (int i=0; i<8; ++i) { bytes[i] = receive_byte(); }

    return (bytes[0] << 56) | (bytes[1] << 48) | (bytes[2] << 40) |
            (bytes[3] << 32) | (bytes[4] << 24) | (bytes[5] << 16) |
            (bytes[6] << 8) | (bytes[7] << 0);
  }

  void send(const char *str) {
    while (char c = *str++) {
      send(c);
    }
  }
};

}

#ifdef USE_LUFA
#include "usb.h"
#define Serial USBSerial
#else
#include "hardware.h"
#define Serial HardwareSerial
#endif
