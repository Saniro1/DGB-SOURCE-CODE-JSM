/**
 * @file DGB_Firmware.ino
 * @brief Dual Gyro Button (DGB) 1000Hz Motion Controller Firmware with Auto Touch Detection
 * 
 * Hardware:
 *   - Waveshare RP2040-Zero (or Raspberry Pi Pico)
 *   - Dual Bosch Sensortec BMI160 6-Axis IMUs (SPI0)
 *   - Capacitive Auto Touch Detection on GP9 / GP12
 * 
 * USB Emulation:
 *   - Sony DualShock 4 HID Gamepad
 *   - 1000 Hz USB Interrupt polling (1 ms bInterval)
 * 
 * JoyShockMapper Compatibility:
 *   - Native auto-detection in JoyShockMapper
 *   - Touchpad finger-contact gates gyro aiming automatically
 */

#include "config.h"
#include "bmi160_spi.h"
#include "touch_sensor.h"
#include "usb_ds4_hid.h"

static DS4InputReport ds4Report;
static uint32_t lastReportUs = 0;
static uint16_t reportCounter = 0;

void setup() {
  // USB Serial debug output
  Serial.begin(115200);

  // Initialize capacitive auto touch detection
  touchSensor.begin();

  // Initialize DualShock 4 HID emulation engine (1000 Hz)
  usbDs4.begin();

  // Initialize Dual BMI160 IMU hardware over SPI0
  imu.begin();

  Serial.println("Pls work");
  Serial.println("waiting for calibration");

  // Calibrate stationary resting offsets
  imu.calibrateRestingOffsets();

  // Clear DS4 report buffer
  memset(&ds4Report, 0, sizeof(DS4InputReport));
  ds4Report.report_id = 0x01;
  ds4Report.lx = 128; // Centered analog sticks
  ds4Report.ly = 128;
  ds4Report.rx = 128;
  ds4Report.ry = 128;
  ds4Report.buttons1 = 0x08; // D-pad center release
  ds4Report.battery = 0x0B;  // 100% full battery

  lastReportUs = micros();
}

void loop() {
  // Ensure precise 1000 Hz timing (1000 microseconds per report)
  uint32_t now = micros();
  if (now - lastReportUs < 1000) {
    return;
  }
  lastReportUs = now;
  reportCounter++;

  // 1. Read Capacitive Auto Touch Sensor
  bool isTouched = touchSensor.readTouch();

  // 2. Read and Fuse Dual BMI160 IMUs (High-Precision 16-bit Gyro + Accel)
  ImuRawData motion;
  imu.readFusedMotion(motion);

  // 3. Populate Motion Sensor fields in DualShock 4 report
  if (isTouched) {
    // When touched, transmit active gyro motion
    ds4Report.gyro_x = motion.gx;
    ds4Report.gyro_y = motion.gy;
    ds4Report.gyro_z = motion.gz;

    // DualShock 4 Touchpad active indicator (Finger down: Bit 7 = 0)
    ds4Report.finger1_id = 0x00 | (reportCounter & 0x7F);
    ds4Report.buttons3 |= 0x02; // Touchpad button flag
  } else {
    // When released, zero out gyro to prevent cursor drift (ratcheting)
    ds4Report.gyro_x = 0;
    ds4Report.gyro_y = 0;
    ds4Report.gyro_z = 0;

    // DualShock 4 Touchpad release indicator (Finger up: Bit 7 = 1)
    ds4Report.finger1_id = 0x80 | (reportCounter & 0x7F);
    ds4Report.buttons3 &= ~0x02;
  }

  // Accelerometer is always active for gravity vector / tilt calibration
  ds4Report.accel_x = motion.ax;
  ds4Report.accel_y = motion.ay;
  ds4Report.accel_z = motion.az;

  ds4Report.timestamp = (uint16_t)(now & 0xFFFF);
  ds4Report.touch_track_num = (uint8_t)(reportCounter & 0xFF);

  // 4. Send 1000 Hz USB HID packet to host
  usbDs4.sendReport(ds4Report);
}
