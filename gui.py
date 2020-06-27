#!/usr/bin/env python3
import platform
from tkinter import *
from tkinter import ttk
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

#Flag to check if UART is connected
connectedFlag = 0

time = "Test"


def getTime():
    global hoursNow, minutesNow, secondsNow
    hoursNow = dt.now().hour
    minutesNow = dt.now().minute
    secondsNow = dt.now().second
    statusLabel['text'] = dt.now().strftime("%H:%M:%S")


root = Tk()
root.title("Time Setter")

photo = PhotoImage(file="images/Crystal_Clear_app_xclock.png")
root.iconphoto(False,photo)

actionsFrame = LabelFrame(root,text="Set current time")
alarmFrame = LabelFrame(root,text="Set alarm")
connectionFrame = LabelFrame(root,text="Connection",padx=10,pady=5)

#Alarm spinboxes
hoursSpin = Spinbox(alarmFrame,from_=0,to=23,font=Font(size=14))
minutesSpin = Spinbox(alarmFrame,from_=0,to=59,font=Font(size=14))
statusLabel = Label(root,text="",relief=SUNKEN,anchor=W)
systemLabel = Label(root,text="",relief=SUNKEN,anchor=W)
getButton = Button(actionsFrame,text="Get current time",command=getTime)

#Establishing serial connection
try:
    ser = serial.Serial('COM4',int(baudrate.get()))
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
            statusLabel['text'] = "Alarm sent"
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

def sendReset():
    try:
        ser.write(b'r')
        ser.write(b'r')
        ser.write(b'r')
        ser.write(b'r')
        statusLabel['text'] = "Reset signal sent"
    except:
        statusLabel['text'] = "Reset signal failed"

def connect():
   #Establishing serial connection
    global ser
    global connectedFlag
    #Checking OS type
    if (platform.system().startswith("Win")):
        serialPort = "COM4"
    else:
        serialPort = "/dev/ttyUSB0"
    #Checking if there is a connection already or establish connection
    if connectedFlag == 1:
        statusLabel['text'] = "Already connected!"
    else:
        try:
            ser = serial.Serial(serialPort,int(baudrate.get()))
            statusLabel['text'] = "Connection established!" + " Baudrate: "+str(baudrate.get())
            connectedFlag = 1
        except:
            statusLabel['text'] = "Could not establish connection"
            connectedFlag = 0
            try:
                ser.close()
            except:
                statusLabel['text'] = "No connection handle"


systemLabel['text'] = platform.system()


clearAlarmButton = Button(alarmFrame,text="Clear alarm",command=clearAlarm,justify="center")

sendButton = Button(actionsFrame,text="Send time",command=sendTime)
setAlarmButton = Button(alarmFrame,text="Set alarm",command=setAlarm,relief=RAISED,justify="center")

resetBtn = Button(actionsFrame,text="Reset system",command=sendReset)

connectBtn = Button(connectionFrame,text="Connect",command=connect)
baudrate = ttk.Combobox(connectionFrame,width="20")
baudrate['values'] = [9600,19200,38400,57600,115200]
baudrate.current(0)

actionsFrame.grid(row=1,column=0,padx=10,pady=5,sticky=N+S)
alarmFrame.grid(row=1,column=1,padx=10,pady=5,sticky=N+S)
connectionFrame.grid(row=1,column=4,padx=10,pady=5,sticky=N+S)
getButton.grid(row=0,column=1,sticky=W)
sendButton.grid(row=0,column=2,sticky=W+E)
resetBtn.grid(row=1,column=0,columnspan=3,sticky=W+E,pady=3)
sendDateBtn.grid(row=0,column=0,sticky=W+E)
connectBtn.grid(row=0,column=0,sticky=W+E)
baudrate.grid(row=1,column=0,sticky=W+E)
hoursSpin.grid(row=0,column=0,padx=3)
minutesSpin.grid(row=0,column=1,padx=3)
setAlarmButton.grid(row=1,column=0,sticky=W,pady=5)
clearAlarmButton.grid(row=1,column=1,sticky=W,pady=5)
statusLabel.grid(row=2,column=0,columnspan=4,sticky=W+E,padx=5)
systemLabel.grid(row=2,column=4,columnspan=3,sticky=W+E,padx=5)


root.mainloop()

