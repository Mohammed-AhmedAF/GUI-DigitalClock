# GUI-DigitalClock
## Summary
A digital clock using the AVR ATMega16 (or ATMega32) and DS-1307 RTC that is set using a UART connection to computer, the time is got and sent using a GUI application. You can also set and clear an alarm using the GUI application.

## Features
* Display current time and date including day of the week.
* Alarm functionality, and keeping the alarm in internal EEPROM.
* Display a message that an alarm is set, even if the system is reset before the alarm event takes place.
* When an alarm event happens, a pin is toggled for a minute. Then the alarm notification disappears from the LCD.
* Stopwatch functionality, controlled by GUI or physical button connected to external interrupt pin.
* Set current current time, date and alarm via GUI application.
* Get temperature of a city (name in python code) from the web, and send it to MCU.
* Clear LCD screen with GUI.
* GUI is working on both Linux and Windows.

## Notice
The packages `python3-tk`, `pyserial` should be installed. tk is used to make the GUI and pyserial is used for UART communication by the GUI app.

## How to compile?
Just put the files of the project in one folder/directory, and then run `make build` from the command line.

Assuming avr-dude is installed on your system, you can run `make load` to load the program into the MCU.

## Todo
Adding cycling of baudrate via external interrupt

## Wiring
### LCD Wiring
| LCD pin | AVR pin |
|---------|---------|
| D0 | PB0 |
| D1 | PB1 |
| D2 | PB2 |
| D3 | PB3 |
| D4 | PB4 |
| D5 | PB5 |
| D6 | PB6 |
| D7 | PB7 |
| RS | PD3 |
| RW | PD4 |
| En | PD5 |


## Pictures
![image](/images/gui_screenshot.png) 
![image](/images/schematic.png)
![image](/images/actual_digital_clock.jpg)
