#!/usr/bin/env python3
from tkinter import *
from datetime import datetime as dt
import serial

#global variables
hoursNow = 0
minutesNow = 0
secondsNow = 0

time = "Test"

def getTime():
    global hoursNow, minutesNow, secondsNow
    hoursNow = dt.now().hour
    minutesNow = dt.now().minute
    secondsNow = dt.now().second
    statusLabel['text'] = dt.now().strftime("%H:%M:%S")

root = Tk()
root.title("Time Setter")
root.geometry("400x80")

photo = PhotoImage(file="Crystal_Clear_app_xclock.png")
root.iconphoto(False,photo)

actionsFrame = LabelFrame(root,text="Action",padx=5,pady=5)

statusLabel = Label(root,text="Hello",relief=SUNKEN,anchor=W)
getButton = Button(actionsFrame,text="Get current time",command=getTime)

#Establishing serial connection
try:
    ser = serial.Serial('/dev/ttyUSB0',9600)
except:
    statusLabel['text'] = "Connection could not be established"

def sendTime():
    getTime()
    try:
        ser.write(int(hoursNow).to_bytes(1,'little'))
        ser.write(int(minutesNow).to_bytes(1,'little'))
        ser.write(int(secondsNow).to_bytes(1,'little'))
    except:
        statusLabel['text'] = "Error sending bytes"


sendButton = Button(actionsFrame,text="Send time!",command=sendTime)

actionsFrame.pack(padx=0,pady=0)
getButton.grid(row=0,column=1,sticky=W)
sendButton.grid(row=0,column=2,sticky=W)
statusLabel.pack(fill=X)

root.mainloop()
