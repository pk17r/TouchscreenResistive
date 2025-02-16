#include "TouchscreenResistive.h"

#define XM 7  // must be an analog pin
#define YP 6  // must be an analog pin
#define XP 19
#define YM 20

const uint16_t xmin = 150, xmax = 930, ymin = 150, ymax = 870;
const uint16_t width = 320, height = 240, Rplate = 310;

TouchscreenResistive ts(XP, XM, YP, YM, Rplate);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // ts.setAdcResolution(10, 5);
  ts.setRotation(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ts.touched())
  {
    TS_Point ts_point = ts.getPoint();
    uint16_t tsx = min((uint16_t)map(ts_point.x, xmin, xmax, 0, width - 1), (uint16_t)(width - 1));
    uint16_t tsy = min((uint16_t)map(ts_point.y, ymin, ymax, 0, height - 1), (uint16_t)(height - 1));

    Serial.print("ts_point x "); Serial.print(ts_point.x);
    Serial.print(", y "); Serial.print(ts_point.y);
    Serial.print("     Touch Pixel ("); Serial.print(tsx);
    Serial.print(","); Serial.print(tsy);
    Serial.println(")");
  }
  else
    delay(100);
}
