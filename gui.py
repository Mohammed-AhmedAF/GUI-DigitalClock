#!/usr/bin/env python3
from tkinter import *
from datetime import datetime as dt
import serial

#global variables
whoursNow = 0
minutesNow = 0
secondsNow = 0
hoursAlarm = 0
minutesAlarm = 0

time = "Test"

def getTime():
    global hoursNow, minutesNow, secondsNow
    hoursNow = dt.now().hour
    minutesNow = dt.now().minute
    secondsNow = dt.now().second
    statusLabel['text'] = dt.now().strftime("%H:%M:%S")

root = Tk()
root.title("Time Setter")
root.maxsize(800,150)

photo = PhotoImage(file="Crystal_Clear_app_xclock.png")
root.iconphoto(False,photo)

actionsFrame = LabelFrame(root,text="Set current time",padx=5,pady=5)
alarmFrame = LabelFrame(root,text="Set alarm",padx=5,pady=5)

#Alarm spinboxes
hoursSpin = Spinbox(alarmFrame,from_=0,to=23)
minutesSpin = Spinbox(alarmFrame,from_=0,to=59)
statusLabel = Label(root,text="",relief=SUNKEN,anchor=W)
getButton = Button(actionsFrame,text="Get current time",command=getTime)

#Establishing serial connection
try:
    ser = serial.Serial('/dev/ttyUSB0',9600)
    statusLabel['text'] = "Connection established!"
except:
    statusLabel['text'] = "Connection could not be established"


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
    ser.write(b'u')
    ser.write(b'u')
    ser.write(b'u')
    ser.write(b'u')

clearAlarmButton = Button(alarmFrame,text="Clear alarm",command=clearAlarm)

sendButton = Button(actionsFrame,text="Send time!",command=sendTime)
setAlarmButton = Button(alarmFrame,text="Set alarm!",command=setAlarm,relief=RAISED)

actionsFrame.grid(row=0,column=0)
alarmFrame.grid(row=0,column=2)
getButton.grid(row=0,column=1,sticky=W)
sendButton.grid(row=0,column=2,sticky=W)
hoursSpin.grid(row=0,column=0,sticky=W)
minutesSpin.grid(row=0,column=1,sticky=W)
setAlarmButton.grid(row=0,column=2,sticky=W)
clearAlarmButton.grid(row=1,column=2,sticky=W)
statusLabel.grid(row=1,column=0,columnspan=5,sticky=W+E)

root.mainloop()
