/*
  Prashant Kumar
  Touchscreen Resistive
  https://github.com/pk17r/TouchscreenResistive

*/

#ifndef TOUCHSCREEN_RESISTIVE_H
#define TOUCHSCREEN_RESISTIVE_H

#include <Arduino.h>


class TS_Point {
  public:
    TS_Point(void) : x(0), y(0), is_touched(false) {}
    TS_Point(int16_t x, int16_t y, bool is_touched) : x(x), y(y), is_touched(is_touched) {}
    int16_t x, y;
    bool is_touched;
  };


class TouchscreenResistive {
  public: 
    TouchscreenResistive(uint8_t XP, uint8_t XM, uint8_t YP, uint8_t YM, uint16_t RPlate = 300);
    void setAdcResolution(uint8_t adc_resolution, uint8_t touch_threshold_percent = 5);
    TS_Point getPoint();
    bool touched();
    void setRotation(uint8_t n) { _rotation = n % 4; }

  private: 
    /* GPIOs
      _xm and _yp need to be analog inputs and digital outputs
      _xp and _yp can be only digital outputs    */
    uint8_t _xm = -1, _yp = -1, _xp = -1, _ym = -1;

    uint8_t _rotation = 1;  // screen rotation  0 & 2 = portrait, 1 & 3 = landscape
    uint16_t _Rplate;   //  resistance in ohms between X- and X+ pins with no touch
    uint16_t _touch_threshold;
    uint16_t _adc_max_reading;
};

#endif    // TOUCHSCREEN_RESISTIVE_H
