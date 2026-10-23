/**
 * @file usb_ds4_hid.cpp
 * @brief Adafruit TinyUSB DualShock 4 HID Report Descriptor Implementation
 */

#include "usb_ds4_hid.h"

UsbDs4Hid usbDs4;

// Sony DualShock 4 compatible Gamepad HID Report Descriptor (139 bytes extracted from UF2)
static const uint8_t ds4_hid_report_descriptor[] = {
  0x05, 0x01,        // Usage Page (Generic Desktop)
  0x09, 0x05,        // Usage (Game Pad)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x06, 0x00, 0xFF,  //   Usage Page (Vendor Specific)
  0x09, 0x01,        //   Usage (Vendor Usage 1)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0xFF,        //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8 bits)
  0x95, 0x02,        //   Report Count (2)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x05, 0x09,        //   Usage Page (Button)
  0x19, 0x01,        //   Usage Minimum (1)
  0x29, 0x20,        //   Usage Maximum (32)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x20,        //   Report Count (32 buttons)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x05, 0x01,        //   Usage Page (Generic Desktop)
  0x09, 0x30,        //   Usage (X)
  0x09, 0x31,        //   Usage (Y)
  0x09, 0x32,        //   Usage (Z)
  0x09, 0x35,        //   Usage (Rz)
  0x09, 0x33,        //   Usage (Rx)
  0x09, 0x34,        //   Usage (Ry)
  0x16, 0x00, 0x80,  //   Logical Minimum (-32768)
  0x26, 0xFF, 0x7F,  //   Logical Maximum (32767)
  0x75, 0x10,        //   Report Size (16)
  0x95, 0x06,        //   Report Count (6)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x06, 0x00, 0xFF,  //   Usage Page (Vendor Specific)
  0x09, 0x20,        //   Usage (0x20)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0xFF, 0xFF,  //   Logical Maximum (65535)
  0x75, 0x20,        //   Report Size (32)
  0x95, 0x01,        //   Report Count (1)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x09, 0x21,        //   Usage (0x21 - Gyro & Accel stream)
  0x16, 0x00, 0x80,  //   Logical Minimum (-32768)
  0x26, 0xFF, 0x7F,  //   Logical Maximum (32767)
  0x75, 0x10,        //   Report Size (16)
  0x95, 0x06,        //   Report Count (6 axes)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x09, 0x22,        //   Usage (0x22 - Touchpad packet)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0xFF, 0x00,  //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x1D,        //   Report Count (29)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x85, 0x02,        //   Report ID (2)
  0x09, 0x23,        //   Usage (0x23)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0xFF, 0x00,  //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x3F,        //   Report Count (63)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x85, 0x03,        //   Report ID (3)
  0x09, 0x24,        //   Usage (0x24)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0xFF, 0x00,  //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x2F,        //   Report Count (47)
  0x91, 0x02,        //   Output (Data, Var, Abs)
  0xC0               // End Collection
};

UsbDs4Hid::UsbDs4Hid() {}

void UsbDs4Hid::begin() {
  _usbHid.setPollInterval(1); // 1 ms polling (1000 Hz)
  _usbHid.setReportDescriptor(ds4_hid_report_descriptor, sizeof(ds4_hid_report_descriptor));
  _usbHid.begin();
}

bool UsbDs4Hid::isReady() {
  return _usbHid.ready();
}

bool UsbDs4Hid::sendReport(const DS4InputReport &report) {
  if (!_usbHid.ready()) return false;
  return _usbHid.sendReport(1, (const void*)&report, sizeof(DS4InputReport));
}
