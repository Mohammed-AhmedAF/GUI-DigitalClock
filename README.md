# GUI-DigitalClock
A digital clock using the AVR ATMega16 (or ATMega32) and DS-1307 RTC that is set using a UART connection to computer, the time is got and sent using GUI application. You can also set and clear an alarm using the GUI application.

* Display current time and date including day of the week.
* Alarm functionality, and keeping the alarm in internal EEPROM.
* Display a message that an alarm is set, even if the system is reset before the alarm event takes place.
* When an alarm event happens, a pin is toggled for a minute. Then the alarm notification disappears from the LCD.
* Set current current time, date and alarm via GUI application.
* Clear LCD screen with GUI.

## How to compile?
Just put the files of the project in one folder/directory, and then run `make build` from the command line.


![image](/gui.png) 

![image](https://user-images.githubusercontent.com/4983317/73939388-04eabe80-48f2-11ea-918b-3a90808dafb3.jpg)
