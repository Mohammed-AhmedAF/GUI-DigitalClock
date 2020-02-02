#!/usr/bin/env python3
from tkinter import *
from tkinter.font import Font
from datetime import datetime as dt
import serial

#global variables
whoursNow = 0
minutesNow = 0
secondsNow = 0
hoursAlarm = 0
minutesAlarm = 0
daysNow = 0
monthsNow = 0
yearsNow = 0

time = "Test"

def getTime():
    global hoursNow, minutesNow, secondsNow
    hoursNow = dt.now().hour
    minutesNow = dt.now().minute
    secondsNow = dt.now().second
    statusLabel['text'] = dt.now().strftime("%H:%M:%S")


root = Tk()
root.title("Time Setter")

photo = PhotoImage(file="Crystal_Clear_app_xclock.png")
root.iconphoto(False,photo)

actionsFrame = LabelFrame(root,text="Set current time",padx=10,pady=5)
alarmFrame = LabelFrame(root,text="Set alarm",padx=10,pady=5)

#Alarm spinboxes
hoursSpin = Spinbox(alarmFrame,from_=0,to=23,font=Font(size=14))
minutesSpin = Spinbox(alarmFrame,from_=0,to=59,font=Font(size=14))
statusLabel = Label(root,text="",relief=SUNKEN,anchor=W)
getButton = Button(actionsFrame,text="Get current time",command=getTime)

#Establishing serial connection
try:
    ser = serial.Serial('/dev/ttyUSB0',9600)
    statusLabel['text'] = "Connection established!"
except:
    statusLabel['text'] = "Connection could not be established"

def sendDayOfWeek():
    statusLabel['text'] = dt.now().weekday()
    try:
        ser.write(b'w')
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        statusLabel['text'] = "Day is " + str(dt.now().weekday())
    except:
        statusLabel['text'] = "Error sending day of the week"

def sendDate():
    global daysNow, monthsNow, yearsNow
    daysNow = dt.now().day
    monthsNow = dt.now().month
    yearsNow = dt.now().year
    try:
        ser.write(b'd')
        ser.write(int(daysNow).to_bytes(1,'little'))
        ser.write(int(monthsNow).to_bytes(1,'little'))
        ser.write(int(yearsNow-2000).to_bytes(1,'little'))
        statusLabel['text'] = "Date sent"
        sendDayOfWeek()
    except:
        statusLabel['text'] = dt.today()
        statusLabel['text'] = "Error sending date"

sendDateBtn = Button(actionsFrame,text="Send date",command=sendDate)

def sendTime():
    getTime()
    try:
        ser.write(b'c')
        ser.write(int(hoursNow).to_bytes(1,'little'))
        ser.write(int(minutesNow).to_bytes(1,'little'))
        ser.write(int(secondsNow).to_bytes(1,'little'))
        statusLabel['text'] = "Current time set"
    except:
        statusLabel['text'] = "Error sending current time"

def setAlarm():
    global hoursAlarm, minutesAlarm
    hoursAlarm = hoursSpin.get()
    minutesAlarm = minutesSpin.get()
    #Checking entered values
    if (int(hoursAlarm) > 23 or int(minutesAlarm) > 59):
        statusLabel['text'] = "Invalid alarm values entered"
    else:
        statusLabel['text'] = str(hoursAlarm) + ':' + str(minutesAlarm)
        #sending hours and minutes of alarm via UART
        try:
            ser.write(b'a')
            ser.write(int(hoursAlarm).to_bytes(1,'little'))
            ser.write(int(minutesAlarm).to_bytes(1,'little'))
            ser.write(b'q')
            statusLabel['text'] = "Alarm sent!"
        except:
            statusLabel['text'] = "Error sending alarm"

def clearAlarm():
    try:
        ser.write(b'u')
        ser.write(b'u')
        ser.write(b'u')
        ser.write(b'u')
    except:
        statusLabel['text'] = "Clear alarm signal failed"


clearAlarmButton = Button(alarmFrame,text="Clear alarm",command=clearAlarm,justify="center")

sendButton = Button(actionsFrame,text="Send time!",command=sendTime)
setAlarmButton = Button(alarmFrame,text="Set alarm!",command=setAlarm,relief=RAISED,justify="center")

actionsFrame.grid(row=0,column=0,padx=5,pady=5)
alarmFrame.grid(row=0,column=1,padx=10,pady=5)
getButton.grid(row=0,column=1,sticky=W)
sendButton.grid(row=0,column=2,sticky=W+E)
sendDateBtn.grid(row=0,column=0,sticky=W+E)
hoursSpin.grid(row=0,column=0,padx=3)
minutesSpin.grid(row=0,column=1,padx=3)
setAlarmButton.grid(row=1,column=0,sticky=W,pady=5)
clearAlarmButton.grid(row=1,column=1,sticky=W,pady=5)
statusLabel.grid(row=1,column=0,columnspan=5,sticky=W+E,padx=5)

root.mainloop()
