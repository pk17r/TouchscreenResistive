# Touchscreen Resistive  
  
Arduino library to read Resistive Touchscreens directly using MCU ADC<br>
**Low Power & Fast touched() response**<br>
  
  
## Description  
  
- Generally resistive touchscreens have 4 channels - X+, X-, Y+ and Y-    
- A resistive touchscreen IC is used to interpret these and communicate to  
  MCU. Examples of these ICs are XPT2046 and TSC2046, etc  
- Usefulness of these ICs is MCU is freed from the task of reading resistive  
  touch inputs from the user, although this is not required. To talk to  
  these ICs the MCU has to allocate resources - SPI lines or extra CS line  
  along with an IRQ gpio. This task can be done good enough by the MCU  
  itself  
- The time cost of directly using MCU for this task is not a lot, under a  
  millisecond to read and process the 4 lines  
- Benefits include saving GPIOs, reduced BOM cost, increased PCB space  

- Library made similar to [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen) by Paul Stoffregen  
- Library uses lot of code from [Adafruit TouchScreen Library](https://github.com/adafruit/Adafruit_TouchScreen)  
  - Benefits over Adafruit library:  
    - additional **fast touched()** function  
    - significant **power savings** by turning LOW touchscreen GPIOs after measurement (can be extra 10mA at 3.3V for 300Ohm Touchscreen)  


## Usage

- XM and YP need to be analog inputs and digital outputs  
- XP and YM can be only digital outputs  
- RPlate is resistance in ohms between X- and X+ pins with no touch  
  - default value is 300  
- **touched()** is fast and returns true/false for touch/no touch  
  - touch_threshold_percent is set to 5% by default  
- **getPoint()** returns touch point with pressure  
  - returns {-1,-1,-1} if no touch  
- provide adc resolution bits and touch threshold with **setAdcResolutionAndThreshold(adc_resolution, touch_threshold_percent)**  
  - default setting is 10 bit for adc_resolution and 5% for touch_threshold_percent  
  - use max 12 bit ADC resolution as int16_t is used as read variable  
  - this function <ins>does not set actual ADC resolution</ins>, set it yourself separately  
- rotation configurable with **setRotation(uint8_t n)**  
  - 0 & 2 = portrait, 1 & 3 = landscape  

## Statistics

Function fetch times in microseconds<br>
<table>
  <tr>
    <th>MCU</th>
    <th>touched()</th>
    <th>getPoint()</th>
  </tr>
  <tr>
    <td>ESP32 S3 @240Mhz</td>
    <td>188 microseconds</td>
    <td>586 microseconds</td>
  </tr>
  <tr>
    <td>Arduino Uno R3 @16MHz</td>
    <td>265 microseconds</td>
    <td>909 microseconds</td>
  </tr>
</table>

Resistive Touchscreen 320x240<br>
![Alt text](resources/resistive_touchscreen_320_240.jpg?raw=true)
<br>
Resistive Touchscreen 320x240 with display<br>
![Alt text](resources/resistive_touchscreen_320_240_with_display.jpg?raw=true)
