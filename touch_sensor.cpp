/**
 * @file touch_sensor.cpp
 * @brief Auto Touch Detection Driver Implementation (RC Time Measurement)
 */

#include "touch_sensor.h"

AutoTouchSensor touchSensor;

AutoTouchSensor::AutoTouchSensor() : _lastState(false) {}

void AutoTouchSensor::begin() {
  pinMode(PIN_TOUCH_SEND, OUTPUT);
  pinMode(PIN_TOUCH_RECV, INPUT);
  digitalWrite(PIN_TOUCH_SEND, LOW);
}

uint32_t AutoTouchSensor::measureChargeCycles() {
  // Ensure receive pin is completely discharged
  digitalWrite(PIN_TOUCH_SEND, LOW);
  delayMicroseconds(10);

  // Assert send pin HIGH and measure charge duration
  digitalWrite(PIN_TOUCH_SEND, HIGH);

  uint32_t count = 0;
  const uint32_t timeout = 1000000;

  while (digitalRead(PIN_TOUCH_RECV) == LOW && count < timeout) {
    count++;
  }

  // Discharge back to LOW
  digitalWrite(PIN_TOUCH_SEND, LOW);
  return count;
}

bool AutoTouchSensor::readTouch() {
  uint32_t cycles = measureChargeCycles();
  bool isTouched = (cycles > TOUCH_THRESHOLD_SAMPLES);

  // Apply hysteresis
  if (isTouched != _lastState) {
    _lastState = isTouched;
  }
  return _lastState;
}
