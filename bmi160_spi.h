/**
 * @file bmi160_spi.h
 * @brief Dual Bosch Sensortec BMI160 IMU SPI Driver
 */

#ifndef BMI160_SPI_H_
#define BMI160_SPI_H_

#include "config.h"

struct ImuRawData {
  int16_t gx;
  int16_t gy;
  int16_t gz;
  int16_t ax;
  int16_t ay;
  int16_t az;
};

struct ImuBiasOffsets {
  int32_t gx;
  int32_t gy;
  int32_t gz;
};

class DualBmi160 {
public:
  DualBmi160();
  bool begin();
  void writeRegister(uint8_t csPin, uint8_t reg, uint8_t val);
  uint8_t readRegister(uint8_t csPin, uint8_t reg);
  void readSensorData(uint8_t csPin, ImuRawData &data);
  void readFusedMotion(ImuRawData &fused);
  void calibrateRestingOffsets();

  ImuBiasOffsets bias1;
  ImuBiasOffsets bias2;

private:
  void initSensor(uint8_t csPin, uint8_t gyrRange);
};

extern DualBmi160 imu;

#endif // BMI160_SPI_H_
