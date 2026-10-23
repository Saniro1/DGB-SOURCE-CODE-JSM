/**
 * @file config.h
 * @brief Hardware and Sensor Configuration for DGB (DIY Gyro Button) Firmware
 * Target MCU: Waveshare RP2040-Zero / Raspberry Pi RP2040
 * IMU Sensors: Dual Bosch Sensortec BMI160 6-Axis IMUs (High-Speed SPI)
 * USB Reporting: Sony DualShock 4 HID Emulation @ 1000 Hz
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

// ==============================================================================
// 1. HARDWARE SPI PINOUT (RP2040 SPI0)
// ==============================================================================
// Default SPI0 mapping on RP2040-Zero:
#define PIN_SPI_MISO            0   // GP0: Master In Slave Out (SDO)
#define PIN_SPI_CS1             1   // GP1: Chip Select for Sensor 1 (Primary Gyro)
#define PIN_SPI_SCK             2   // GP2: Serial Clock (SCK)
#define PIN_SPI_MOSI            3   // GP3: Master Out Slave In (SDI / SDA)
#define PIN_SPI_CS2             5   // GP5: Chip Select for Sensor 2 (Secondary Gyro)

#define SPI_CLOCK_SPEED_HZ      10000000  // 10 MHz High-Speed SPI Bus

// ==============================================================================
// 2. CAPACITIVE AUTO TOUCH DETECTION PINOUT
// ==============================================================================
// RC charge-time capacitive touch detection circuit.
// Connect a 1MΩ - 10MΩ resistor between PIN_TOUCH_SEND and PIN_TOUCH_RECV.
// Connect conductive copper tape / touch foil to PIN_TOUCH_RECV.
#define PIN_TOUCH_SEND          9   // GP9: Pulse Send Pin (OUTPUT)
#define PIN_TOUCH_RECV          12  // GP12: Capacitive Sensor Foil Pin (INPUT)

// Touch threshold: higher value requires stronger contact
#define TOUCH_THRESHOLD_SAMPLES 35  // Cycle counter threshold for touch contact
#define TOUCH_HYSTERESIS        5   // Noise rejection margin

// ==============================================================================
// 3. BMI160 SENSOR TUNING & REGISTER CONFIGURATION
// ==============================================================================
// BMI160 Command Register:
#define BMI160_REG_CMD          0x7E
#define BMI160_CMD_ACC_NORMAL   0x15
#define BMI160_CMD_GYR_NORMAL   0x11

// Sensor Registers:
#define BMI160_REG_CHIP_ID      0x00
#define BMI160_CHIP_ID_VALUE    0xD1

#define BMI160_REG_DATA_8       0x0C  // Start of GYR_X_L (6 bytes gyro, 6 bytes accel)
#define BMI160_REG_ACC_CONF     0x40  // Normal bandwidth, 800 Hz ODR (0x2C)
#define BMI160_REG_ACC_RANGE    0x41  // ±16g full scale (0x0C)
#define BMI160_REG_GYR_CONF     0x42  // Normal bandwidth, 1600 Hz ODR (0x0D)
#define BMI160_REG_GYR_RANGE_1  0x43  // Sensor 1: ±500 dps (0x02)
#define BMI160_REG_GYR_RANGE_2  0x43  // Sensor 2: ±2000 dps (0x04)

// Output Reporting:
#define USB_POLL_RATE_HZ        1000  // 1000 Hz (1 ms interval) DualShock 4 HID report
#define CALIBRATION_SAMPLES     1000  // Samples collected at boot while stationary

#endif // CONFIG_H_
