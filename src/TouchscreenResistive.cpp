/*
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

#include <stdint.h>
#include "TouchscreenResistive.h"
#include <Arduino.h>

TouchscreenResistive::TouchscreenResistive(uint8_t XP, uint8_t XM, uint8_t YP, uint8_t YM, uint16_t RPlate)
{
  _xm = XM;
  _yp = YP;
  _xp = XP;
  _ym = YM;
  _Rplate = RPlate;
  setAdcResolutionAndThreshold(10);   // set adc to 10 bit resolution
}

void TouchscreenResistive::setAdcResolutionAndThreshold(uint8_t adc_resolution, uint8_t touch_threshold_percent)
{
  _adc_max_reading = pow(2, adc_resolution) - 1;
  _touch_threshold = double(touch_threshold_percent) / 100 * _adc_max_reading;
}

TsPoint TouchscreenResistive::getPoint()
{
  const int kMaxRunCounts = 4;

  // x measurement

  pinMode(_xp, INPUT);
  pinMode(_xm, INPUT);
  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);
  digitalWrite(_yp, HIGH);
  digitalWrite(_ym, LOW);
  // Serial.println("_yp = HIGH     _ym = GND");
  delayMicroseconds(10);
  int16_t x1 = analogRead(_xm);
  delayMicroseconds(10);
  int16_t x2 = analogRead(_xm);
  int run_counts = 0;
  while(abs(x2 - x1) > 10 && run_counts < kMaxRunCounts) {
    run_counts++;
    x1 = x2;
    delayMicroseconds(10);
    x2 = analogRead(_xm);
  }
  int16_t x = (x1 + x2) / 2;

  // y measurement

  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);
  digitalWrite(_xp, HIGH);
  digitalWrite(_xm, LOW);
  // Serial.println("_xp = HIGH     _xm = GND");
  delayMicroseconds(10);
	int16_t y1 = analogRead(_yp);
  delayMicroseconds(10);
  int16_t y2 = analogRead(_yp);
  run_counts = 0;
  while(abs(y2 - y1) > 10 && run_counts < kMaxRunCounts) {
    run_counts++;
    y1 = y2;
    delayMicroseconds(10);
    y2 = analogRead(_yp);
  }
  int16_t y = (y1 + y2) / 2;

  // z measurement

  // Set X+ to ground
  // Set Y- to VCC
  // Hi-Z X- and Y+
  pinMode(_yp, INPUT);
  pinMode(_xm, INPUT);
  pinMode(_ym, OUTPUT);
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);
  digitalWrite(_ym, HIGH);
  // Serial.println("_ym = HIGH     _xm = GND");
  delayMicroseconds(10);

  int16_t z1 = analogRead(_xm);
  int16_t z2 = analogRead(_yp);
  int16_t z = 0;
  // Serial.printf("z1 = %d, z2 = %d \n", z1, z2);

  // turn gpios low to conserve power
  digitalWrite(_ym, HIGH);

  if (_Rplate != 0) {
    // now read the x
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= x;
    rtouch *= _Rplate;
    rtouch /= _adc_max_reading;

    z = rtouch;
  } else {
    z = (_adc_max_reading - (z2 - z1));
  }

  if(z > 0) {
		switch (_rotation) {
		  case 0:
			x = _adc_max_reading - y;
			y = x;
			break;
		  case 1:
			x = x;
			y = y;
			break;
		  case 2:
			x = y;
			y = _adc_max_reading - x;
			break;
		  default: // 3
			x = _adc_max_reading - x;
			y = _adc_max_reading - y;
		}
    return TsPoint(x, y, z);
	}
  else {
    return TsPoint();
  }
}

bool TouchscreenResistive::touched()
{
	// Set X+ to ground
  // Set Y- to VCC
  // Hi-Z X- and Y+
  pinMode(_yp, INPUT);
  pinMode(_xm, INPUT);
  pinMode(_ym, OUTPUT);
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);
  digitalWrite(_ym, HIGH);
  delayMicroseconds(10);
  int16_t z1 = analogRead(_xm);
  int16_t z2 = analogRead(_yp);

  // turn gpios low to conserve power
  digitalWrite(_ym, LOW);

  if(z1 > _touch_threshold || z2 < _adc_max_reading - _touch_threshold)
    return true;
  else
    return false;
}



