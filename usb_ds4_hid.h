/**
 * @file usb_ds4_hid.h
 * @brief Adafruit TinyUSB DualShock 4 HID Report Descriptor & Sender
 */

#ifndef USB_DS4_HID_H_
#define USB_DS4_HID_H_

#include "config.h"
#include <Adafruit_TinyUSB.h>

// DualShock 4 64-byte Input Report Structure
struct DS4InputReport {
  uint8_t report_id;      // 0x01
  uint8_t lx;             // Left stick X (0-255, 128 = center)
  uint8_t ly;             // Left stick Y
  uint8_t rx;             // Right stick X
  uint8_t ry;             // Right stick Y
  uint8_t buttons1;       // D-pad (4 bits) + Square, Cross, Circle, Triangle
  uint8_t buttons2;       // L1, R1, L2, R2, Share, Options, L3, R3
  uint8_t buttons3;       // PS button, Touchpad button, report counter
  uint8_t l2_trigger;     // Analog L2 (0-255)
  uint8_t r2_trigger;     // Analog R2 (0-255)
  uint16_t timestamp;     // Microsecond timer tick
  uint8_t battery;        // Battery level status

  // 16-bit signed motion sensors (DualShock 4 orientation):
  int16_t gyro_x;         // Roll
  int16_t gyro_y;         // Yaw
  int16_t gyro_z;         // Pitch
  int16_t accel_x;
  int16_t accel_y;
  int16_t accel_z;

  // Touchpad packet structure:
  uint8_t touch_track_num;// Track packet counter
  uint8_t finger1_id;     // Bit 7: 0 = finger down, 1 = finger up
  uint8_t finger1_x_low;
  uint8_t finger1_x_high_y_low;
  uint8_t finger1_y_high;

  uint8_t reserved[32];   // Padding to full DualShock 4 packet size
} __attribute__((packed));

class UsbDs4Hid {
public:
  UsbDs4Hid();
  void begin();
  bool sendReport(const DS4InputReport &report);
  bool isReady();

private:
  Adafruit_USBD_HID _usbHid;
};

extern UsbDs4Hid usbDs4;

#endif // USB_DS4_HID_H_
