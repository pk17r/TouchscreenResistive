#include <stdint.h>
/*
  Prashant Kumar
  Touchscreen Resistive
  https://github.com/pk17r/TouchscreenResistive


*/

#include "TouchscreenResistive.h"

TouchscreenResistive::TouchscreenResistive(uint8_t XP, uint8_t XM, uint8_t YP, uint8_t YM, uint16_t RPlate)
{
  _xm = XM;
  _yp = YP;
  _xp = XP;
  _ym = YM;
  _Rplate = RPlate;
  setAdcResolution(10);   // set adc to 10 bit resolution
  // Serial.printf("_xp %d, _xm %d, _yp %d, _ym %d\n", _xp, _xm, _yp, _ym);
}

void TouchscreenResistive::setAdcResolution(uint8_t adc_resolution, uint8_t touch_threshold_percent)
{
  analogReadResolution(adc_resolution);
  _adc_max_reading = pow(2, adc_resolution) - 1;
  _touch_threshold = double(touch_threshold_percent) / 100 * _adc_max_reading;
  Serial.printf("_adc_max_reading %d, _touch_threshold %d\n", _adc_max_reading, _touch_threshold);
}

TS_Point TouchscreenResistive::getPoint()
{
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
  int16_t x = (analogRead(_xm) + x1) / 2;

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
  int16_t y = (analogRead(_yp) + y1) / 2;

  // turn low to conserve power
  digitalWrite(_xp, HIGH);

  if(x > _touch_threshold || y > _touch_threshold) {
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
    return TS_Point(x, y, true);
	}
  else {
    return TS_Point();
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
  int16_t x = analogRead(_xm);
  int16_t y = analogRead(_yp);

  // turn low to conserve power
  digitalWrite(_ym, LOW);

  if(x > _touch_threshold || y < _adc_max_reading - _touch_threshold)
    return true;
  else
    return false;
}



