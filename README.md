# Touchscreen Resistive  
  
Arduino library to read Resistive Touchscreens directly using MCU ADC  
  
  
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
