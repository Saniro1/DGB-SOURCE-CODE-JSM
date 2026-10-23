/**
 * @file touch_sensor.h
 * @brief Auto Touch Detection Driver for Capacitive Grip / Button Foil
 */

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

#include "config.h"

class AutoTouchSensor {
public:
  AutoTouchSensor();
  void begin();
  bool readTouch();
  uint32_t measureChargeCycles();

private:
  bool _lastState;
};

extern AutoTouchSensor touchSensor;

#endif // TOUCH_SENSOR_H_
