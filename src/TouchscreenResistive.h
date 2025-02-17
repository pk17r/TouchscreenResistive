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

#ifndef TOUCHSCREEN_RESISTIVE_H
#define TOUCHSCREEN_RESISTIVE_H

#include <Arduino.h>


class TsPoint {
  public:
    TsPoint(void) : x(-1), y(-1), z(-1) {}
    TsPoint(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {}
    int16_t x, y, z;
  };


class TouchscreenResistive {
  public: 
    TouchscreenResistive(uint8_t XP, uint8_t XM, uint8_t YP, uint8_t YM, uint16_t RPlate = 300);
    void setAdcResolutionAndThreshold(uint8_t adc_resolution, uint8_t touch_threshold_percent = 5);   // max 12 bit ADC resolution as int16_t is used as read variable
    TsPoint getPoint();
    bool touched();
    void setRotation(uint8_t n) { _rotation = n % 4; }

  private: 
    /* GPIOs
      _xm and _yp need to be analog inputs and digital outputs
      _xp and _ym can be only digital outputs    */
    uint8_t _xm = -1, _yp = -1, _xp = -1, _ym = -1;

    uint8_t _rotation = 1;  // screen rotation  0 & 2 = portrait, 1 & 3 = landscape
    uint16_t _Rplate;   //  resistance in ohms between X- and X+ pins with no touch
    uint16_t _touch_threshold;
    uint16_t _adc_max_reading;
};

#endif    // TOUCHSCREEN_RESISTIVE_H
