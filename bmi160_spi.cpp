/**
 * @file bmi160_spi.cpp
 * @brief Dual Bosch Sensortec BMI160 IMU SPI Driver Implementation
 */

#include "bmi160_spi.h"
#include <SPI.h>

DualBmi160 imu;

DualBmi160::DualBmi160() {
  bias1 = {0, 0, 0};
  bias2 = {0, 0, 0};
}

void DualBmi160::writeRegister(uint8_t csPin, uint8_t reg, uint8_t val) {
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(csPin, LOW);
  SPI.transfer(reg & 0x7F); // Bit 7 = 0 for write
  SPI.transfer(val);
  digitalWrite(csPin, HIGH);
  SPI.endTransaction();
}

uint8_t DualBmi160::readRegister(uint8_t csPin, uint8_t reg) {
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(csPin, LOW);
  SPI.transfer(reg | 0x80); // Bit 7 = 1 for read
  uint8_t val = SPI.transfer(0x00);
  digitalWrite(csPin, HIGH);
  SPI.endTransaction();
  return val;
}

void DualBmi160::initSensor(uint8_t csPin, uint8_t gyrRange) {
  // Configure ODR and filters
  writeRegister(csPin, BMI160_REG_GYR_CONF, 0x0D); // 1600 Hz ODR, Normal Bandwidth
  delay(10);
  writeRegister(csPin, BMI160_REG_GYR_RANGE_1, gyrRange); // Full scale range
  delay(10);
  writeRegister(csPin, BMI160_REG_ACC_CONF, 0x2C); // 800 Hz ODR, Normal Bandwidth
  delay(10);
  writeRegister(csPin, BMI160_REG_ACC_RANGE, 0x0C); // ±16g range
  delay(10);

  // Power on Accel and Gyro engines into Normal Mode
  writeRegister(csPin, BMI160_REG_CMD, BMI160_CMD_ACC_NORMAL);
  delay(100);
  writeRegister(csPin, BMI160_REG_CMD, BMI160_CMD_GYR_NORMAL);
  delay(100);
}

bool DualBmi160::begin() {
  // Set CS pins as OUTPUT and HIGH
  pinMode(PIN_SPI_CS1, OUTPUT);
  pinMode(PIN_SPI_CS2, OUTPUT);
  digitalWrite(PIN_SPI_CS1, HIGH);
  digitalWrite(PIN_SPI_CS2, HIGH);
  delay(10);

  // Initialize RP2040 SPI0 pins
  SPI.setRX(PIN_SPI_MISO);
  SPI.setCS(PIN_SPI_CS1);
  SPI.setSCK(PIN_SPI_SCK);
  SPI.setTX(PIN_SPI_MOSI);
  SPI.begin();

  // Initialize Sensor 1 (CS1 on GP1, ±500 dps)
  initSensor(PIN_SPI_CS1, 0x02);

  // Initialize Sensor 2 (CS2 on GP5, ±2000 dps)
  initSensor(PIN_SPI_CS2, 0x04);

  delay(250);
  return true;
}

void DualBmi160::readSensorData(uint8_t csPin, ImuRawData &data) {
  uint8_t raw[12];
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(csPin, LOW);
  SPI.transfer(BMI160_REG_DATA_8 | 0x80); // Burst read starting from register 0x0C
  for (int i = 0; i < 12; i++) {
    raw[i] = SPI.transfer(0x00);
  }
  digitalWrite(csPin, HIGH);
  SPI.endTransaction();

  // Little-endian signed 16-bit
  data.gx = (int16_t)((raw[1] << 8) | raw[0]);
  data.gy = (int16_t)((raw[3] << 8) | raw[2]);
  data.gz = (int16_t)((raw[5] << 8) | raw[4]);
  data.ax = (int16_t)((raw[7] << 8) | raw[6]);
  data.ay = (int16_t)((raw[9] << 8) | raw[8]);
  data.az = (int16_t)((raw[11] << 8) | raw[10]);
}

void DualBmi160::readFusedMotion(ImuRawData &fused) {
  ImuRawData d1, d2;
  readSensorData(PIN_SPI_CS1, d1);
  readSensorData(PIN_SPI_CS2, d2);

  // Apply zero-rate calibration offsets
  d1.gx -= (int16_t)bias1.gx;
  d1.gy -= (int16_t)bias1.gy;
  d1.gz -= (int16_t)bias1.gz;

  d2.gx -= (int16_t)bias2.gx;
  d2.gy -= (int16_t)bias2.gy;
  d2.gz -= (int16_t)bias2.gz;

  // Fuse both sensors (average readings to cancel uncorrelated sensor noise):
  fused.gx = (d1.gx + d2.gx) / 2;
  fused.gy = (d1.gy + d2.gy) / 2;
  fused.gz = (d1.gz + d2.gz) / 2;

  fused.ax = (d1.ax + d2.ax) / 2;
  fused.ay = (d1.ay + d2.ay) / 2;
  fused.az = (d1.az + d2.az) / 2;
}

void DualBmi160::calibrateRestingOffsets() {
  int64_t sumGx1 = 0, sumGy1 = 0, sumGz1 = 0;
  int64_t sumGx2 = 0, sumGy2 = 0, sumGz2 = 0;

  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    ImuRawData d1, d2;
    readSensorData(PIN_SPI_CS1, d1);
    readSensorData(PIN_SPI_CS2, d2);

    sumGx1 += d1.gx;
    sumGy1 += d1.gy;
    sumGz1 += d1.gz;

    sumGx2 += d2.gx;
    sumGy2 += d2.gy;
    sumGz2 += d2.gz;

    delayMicroseconds(1000);
  }

  bias1.gx = sumGx1 / CALIBRATION_SAMPLES;
  bias1.gy = sumGy1 / CALIBRATION_SAMPLES;
  bias1.gz = sumGz1 / CALIBRATION_SAMPLES;

  bias2.gx = sumGx2 / CALIBRATION_SAMPLES;
  bias2.gy = sumGy2 / CALIBRATION_SAMPLES;
  bias2.gz = sumGz2 / CALIBRATION_SAMPLES;
}
