#!/usr/bin/env python3
import platform
from tkinter import *
from tkinter import ttk
from tkinter.font import Font
from datetime import datetime as dt
import serial
import requests
import json

#global variables
whoursNow = 0
minutesNow = 0
secondsNow = 0
hoursAlarm = 0
minutesAlarm = 0
daysNow = 0
monthsNow = 0
yearsNow = 0
ser = serial.Serial()
baudrate = 0
serialPort = str()

#Flag to check if UART is connected
connectedFlag = 0

time = "Test"

def sendFrame(frame):
    ser.write(frame[0])
    ser.write(frame[1])
    ser.write(frame[2])
    ser.write(frame[3])

def getTime():
    global hoursNow, minutesNow, secondsNow
    hoursNow = dt.now().hour
    minutesNow = dt.now().minute
    secondsNow = dt.now().second
    updateStatusbar(dt.now().strftime("%H:%M:%S"))

def prepareFrame(a,b,c,d):
    frame = []
    frame[0] = a.to_bytes(1,'little');
    frame[1] = b.to_bytes(1,'little');
    frame[2] = c.to_bytes(1,'little');
    frame[3] = d.to_bytes(1,'little');
    return frame


def updateStatusbar(message):
    statusLabel['text'] = message

root = Tk()
root.title("Time Setter")
root.resizable(width=False,height=False)

photo = PhotoImage(file="images/Crystal_Clear_app_xclock.png")
root.iconphoto(False,photo)

actionsFrame = LabelFrame(root,text="Set current time")
alarmFrame = LabelFrame(root,text="Set alarm")
temperatureFrame = LabelFrame(root,text="Temperature")
connectionFrame = LabelFrame(root,text="Connection",padx=10,pady=5)

#Alarm spinboxes
hoursSpin = Spinbox(alarmFrame,from_=0,to=23,font=Font(size=14))
minutesSpin = Spinbox(alarmFrame,from_=0,to=59,font=Font(size=14))
statusLabel = Label(root,text="",relief=SUNKEN,anchor=W)
systemLabel = Label(root,text="",relief=SUNKEN,anchor=W)
getButton = Button(actionsFrame,text="Get current time",command=getTime)

def sendDayOfWeek():
    statusLabel['text'] = dt.now().weekday()
    try:
        ser.write(b'w')
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        ser.write(dt.now().weekday().to_bytes(1,'little'))
        updateStatusbar("Day is " + str(dt.now().weekday()))
    except:
        updateStatusbar("Error sending day of the week")

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
        updateStatusbar("Date sent")
        sendDayOfWeek()
    except:
        updateStatusbar("Error sending date")

sendDateBtn = Button(actionsFrame,text="Send date",command=sendDate)

def sendTime():
    getTime()
    try:
        ser.write(b'c')
        ser.write(int(hoursNow).to_bytes(1,'little'))
        ser.write(int(minutesNow).to_bytes(1,'little'))
        ser.write(int(secondsNow).to_bytes(1,'little'))
        updateStatusbar("Current time set")
    except:
        updateStatusbar("Error sending current time")

def setAlarm():
    global hoursAlarm, minutesAlarm
    hoursAlarm = hoursSpin.get()
    minutesAlarm = minutesSpin.get()
    #Checking entered values
    if (int(hoursAlarm) > 23 or int(minutesAlarm) > 59):
        updateStatusbar("Invalid alarm values entered")
    else:
        updateStatusbar(str(hoursAlarm) + ':' + str(minutesAlarm))
        #sending hours and minutes of alarm via UART
        try:
            ser.write(b'a')
            ser.write(int(hoursAlarm).to_bytes(1,'little'))
            ser.write(int(minutesAlarm).to_bytes(1,'little'))
            ser.write(b'q')
            updateStatusbar("Alarm sent")
        except:
            updateStatusbar("Error sending alarm")

def clearAlarm():
    try:
        ser.write(b'u')
        ser.write(b'u')
        ser.write(b'u')
        ser.write(b'u')
    except:
        updateStatusbar("Clear alarm signal failed")

def sendReset():
    try:
        ser.write(b'r')
        ser.write(b'r')
        ser.write(b'r')
        ser.write(b'r')
        updateStatusbar("Reset signal sent")
    except:
        updateStatusbar("Reset signal failed")

def connectByUART():
   #Establishing serial connection
    global ser
    global baudrate
    global serialPort
    #Checking OS type
    if not (platform.system().startswith("Win")):
        serialPort = "/dev/ttyUSB0"
    else:
        serialPort = portCmbox.get()
    #Checking if there is a connection already or establish connection
    if not ser.is_open:
        try:
            baudrate = baudrateCmbox.get()
            ser = serial.Serial(serialPort,baudrate)
            updateStatusbar("Connection established!" + " Baudrate: "+str(baudrateCmbox.get()) + " on " + serialPort)
        except:
            updateStatusbar("Could not establish connection "  + "on " + serialPort + " with baudrate " + str(baudrateCmbox.get()))
    else:
        if baudrate!=baudrateCmbox.get() or serialPort!=portCmbox.get():
            try:
                baudrate = int(baudrateCmbox.get())
                ser.close()
                connectByUART()
            except:
                updateStatusbar("No connection handle")
        else:
                updateStatusbar("Already connected!")


systemLabel['text'] = platform.system()


clearAlarmButton = Button(alarmFrame,text="Clear alarm",command=clearAlarm,justify="center")

sendButton = Button(actionsFrame,text="Send time",command=sendTime)
setAlarmButton = Button(alarmFrame,text="Set alarm",command=setAlarm,relief=RAISED,justify="center")

resetBtn = Button(actionsFrame,text="Reset system",command=sendReset)

#Connection elements
portLabel = Label(connectionFrame,text="Port: ")
baudrateLabel = Label(connectionFrame,text="Baudrate: ")
baudrateCmbox = ttk.Combobox(connectionFrame,width="20",state="readonly")
baudrateCmbox['values'] = [9600,19200,38400,57600,115200]
baudrateCmbox.current(0)

connectBtn = Button(connectionFrame,text="Connect",command=connectByUART)
portCmbox = ttk.Combobox(connectionFrame,values=["COM3","COM4"],state="readonly")
portCmbox.current(0)

#Connect automatically on program startup
connectByUART()

temperatureDisplay = Label(temperatureFrame,text="",anchor=W)
def sendTemperature():
    apiURL = "http://api.openweathermap.org/data/2.5/weather?q=Ismailia&appid=000ea0deb2c5a6f1cb107ca1353ea84d"
    response = requests.get(apiURL)
    x = response.json()
    if x["cod"] != "404":
        y = x["main"]
        temperatureReceived = int(y["temp"] - 273.15)
        temperatureDisplay["text"] = str(temperatureReceived) + " Celsius"
        try:
            ser.write(b't')
            ser.write(int(int(temperatureReceived)).to_bytes(1,'little'))
            ser.write(b't')
            ser.write(b't')
            updateStatusbar("Temperature sent")
        except:
            updateStatusbar("Temperature failed")



sendTemperatureBtn = Button(temperatureFrame,text="Get & send Temp.",command=sendTemperature)

#Containers placement
actionsFrame.grid(row=1,column=0,padx=10,pady=5,sticky=N+S)
alarmFrame.grid(row=1,column=1,padx=10,pady=5,sticky=N+S)
temperatureFrame.grid(row=1,column=2,padx=10,pady=5,sticky=N+S)
connectionFrame.grid(row=1,column=3,padx=10,pady=5,sticky=N+S)

#Temperature elements
temperatureDisplay.grid(row=0,column=0,padx=10,pady=5,sticky=N+S)
sendTemperatureBtn.grid(row=1,column=0,padx=10,pady=5,sticky=N+S)

#Time elements placements
getButton.grid(row=0,column=1,sticky=W)
sendButton.grid(row=0,column=2,sticky=W+E)
resetBtn.grid(row=1,column=0,columnspan=3,sticky=W+E,pady=3)
sendDateBtn.grid(row=0,column=0,sticky=W+E)

#Connection elements palcements
connectBtn.grid(row=2,column=0,sticky=W+E,pady=5)
portLabel.grid(row=0,column=0,stick=W)
baudrateLabel.grid(row=1,column=0,sticky=W)
baudrateCmbox.grid(row=1,column=1)
portCmbox.grid(row=0,column=1,sticky=W+E)

#Alarm elements placements
hoursSpin.grid(row=0,column=0,padx=3)
minutesSpin.grid(row=0,column=1,padx=3)
setAlarmButton.grid(row=1,column=0,sticky=W+E,pady=5)
clearAlarmButton.grid(row=1,column=1,sticky=W+E,pady=5)

#Status and system bars
statusLabel.grid(row=2,column=0,columnspan=3,sticky=W+E,padx=10)
systemLabel.grid(row=2,column=3,columnspan=2,sticky=W+E,padx=10)

root.mainloop()
