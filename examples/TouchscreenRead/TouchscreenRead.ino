#include "TouchscreenResistive.h"
/*
  TouchscreenRead Example
  Prashant Kumar
  Touchscreen Resistive
  https://github.com/pk17r/TouchscreenResistive

  - XM and YP need to be analog inputs and digital outputs
  - XP and YM can be only digital outputs
  - RPlate is resistance in ohms between X- and X+ pins with no touch
    - default value is 300
  - touched() is fast and returns true/false for touch/no touch
    - touch_threshold_percent is set to 5% by default
  - getPoint() returns touch point with pressure
    - returns {-1,-1,-1} if no touch
  - provide adc resolution bits and touch threshold with setAdcResolutionAndThreshold(uint8_t adc_resolution, uint8_t touch_threshold_percent)
    - default setting is 10 bit for adc_resolution and 5% for touch_threshold_percent
    - use max 12 bit ADC resolution as int16_t is used as read variable
    - this function does not set actual ADC resolution, set it yourself separately
  - rotation configurable with setRotation(uint8_t n)
    - 0 & 2 = portrait, 1 & 3 = landscape
*/

#define XM 7  // must be an analog pin
#define YP 3  // must be an analog pin
#define XP 19
#define YM 20

const uint16_t xmin = 41, xmax = 881, ymin = 113, ymax = 914;
const uint16_t width = 320, height = 240;

TouchscreenResistive ts(XP, XM, YP, YM);    // default Rplate = 300 Ohms (5th argument)

// Statistics variables
unsigned long micros_fetch_time_us = 0, touched_fetch_time_us = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  // ADC resolution and touch_threshold_percent can be changed using (default setting = 10 bit)
  // analogReadResolution(adc_resolution);
  // ts.setAdcResolutionAndThreshold( /* adc_resolution = */ 10);
  // ts.setAdcResolutionAndThreshold( /* adc_resolution = */ 10, /* touch_threshold_percent = */ 5);

  // Rotation of touchscreen can be changed using:
  ts.setRotation(3);

  // Statistics
  unsigned long randomVar;
  unsigned long t1 = micros();
  for (int i = 0; i < 1000; i++) {
    randomVar = micros();
  }
  unsigned long t2 = micros();
  randomVar += 1;
  micros_fetch_time_us = round(double(t2-t1) / 1000.0);
  Serial.print("micros_fetch_time_us = "); Serial.println(micros_fetch_time_us);
  ts.touched();   // first fetch is slow
  ts.touched();
  t1 = micros();
  ts.touched();
  t2 = micros();
  touched_fetch_time_us = t2 - t1 - micros_fetch_time_us;
}

unsigned long last_read_millis = 0;

void read_touch_point() {
  unsigned long t1 = micros();
  TsPoint ts_point = ts.getPoint();
  unsigned long t2 = micros();
  unsigned long getPoint_fetch_time_us = t2 - t1 - micros_fetch_time_us;

  last_read_millis = millis();
  Serial.print(last_read_millis);

  // Statistics
  Serial.print("    touched_fetch_time_us "); Serial.print(touched_fetch_time_us);
  Serial.print("    getPoint_fetch_time_us "); Serial.print(getPoint_fetch_time_us);

  // touchscreen readings from library
  Serial.print("    ts_point x "); Serial.print(ts_point.x);
  Serial.print(", y "); Serial.print(ts_point.y);
  Serial.print(", z "); Serial.print(ts_point.z);

  if(ts_point.z > 0) {
    uint16_t tsx = max(min((int16_t)map(ts_point.x, xmin, xmax, 0, width - 1), (int16_t)(width - 1)), (int16_t)0);
    uint16_t tsy = max(min((int16_t)map(ts_point.y, ymin, ymax, 0, height), (int16_t)(height - 1)), (int16_t)0);
    Serial.print("     Touch Pixel ("); Serial.print(tsx);
    Serial.print(","); Serial.print(tsy);
    Serial.println(")");
  }
  else {
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long t1 = micros();
  if(ts.touched()) {
    unsigned long t2 = micros();
    touched_fetch_time_us = t2 - t1 - micros_fetch_time_us;
    read_touch_point();
  }
  else if(millis() - last_read_millis > 1000) {
    read_touch_point();
  }
  delay(100);
}
