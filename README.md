# GUI-DigitalClock
## Summary
A digital clock using the AVR ATMega16 (or ATMega32) and DS-1307 RTC that is set using a UART connection to computer, the time is got and sent using a GUI application. You can also set and clear an alarm using the GUI application.

## Features
* Display current time and date including day of the week.
* Alarm functionality, and keeping the alarm in internal EEPROM.
* Display a message that an alarm is set, even if the system is reset before the alarm event takes place.
* When an alarm event happens, a pin is toggled for a minute. Then the alarm notification disappears from the LCD.
* Set current current time, date and alarm via GUI application.
* Get temperature of a city (name in python code) from the web, and send it to MCU.
* Clear LCD screen with GUI.
* GUI is working on both Linux and Windows.

## Notice
The packages `python3-tk`, `pyserial` should be installed. tk is used to make the GUI and pyserial is used for UART communication by the GUI app.

## How to compile?
Just put the files of the project in one folder/directory, and then run `make build` from the command line.

Assuming avr-dude is installed on your system, you can run `make load` to load the program into the MCU.

## Project status
Having problems with sending alarm and temperature values to microcontroller. The transmission is broken or LCD doesn't update.

Update: Never mind, it was a wiring issue!

## Pictures
![image](/images/gui_screenshot.png) 
![image](/images/schematic.png)
![image](/images/acutal_digital_clock.jpg)
