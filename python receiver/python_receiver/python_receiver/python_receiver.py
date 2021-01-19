import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import struct
from scipy.io import wavfile
from tkinter import *  
from tkinter import messagebox
from tkinter import simpledialog
from playsound import playsound
import pyaudio

p = pyaudio.PyAudio()
stream = p.open(format=p.get_format_from_width(1)  , channels = 1 , rate = 16000, output = TRUE)
top = Tk()  
buff_size =1024
arr = []
temp_arr = []
run = 0
start_state = 0
counter = 0
quit_flag = 0
s = serial.Serial('COM5' , baudrate = 230400)
messagebox.showinfo("Hello!" , "Port initialized!")

def read_data():
    global s
    global arr
    global temp_arr
    global p 
    global stream
    global counter
    global quit_flag
    if (quit_flag == 1):
        return
    else:

        if(run):
            sound = s.read(buff_size)
            arr.append(sound)
            stream.write(sound)
        else:
            s.read(buff_size)
 
        top.after(10, read_data)

def start_reading():
    global run
    global start_state
    global s
    run = 1
    

def stop_reading():
    global run
    global arr
    global start_state
    run = 0

def quit_app():
   
    global run
    global s
    global arr
    global p
    global stream
    global quit_flag
    
    s.close()
    quit_flag = 1
    stream.stop_stream()
    stream.close()
    p.terminate()
    
    if (run == 0):
        arr1 = [] 

        for i in arr:
            for j in i:
                arr1.append(j)


        arr1 = np.array(arr1)
        arr1 = arr1/np.max(arr1)
        arr1 = (arr1 - np.mean(arr1))*32767
        arr1 = np.asarray(arr1 , dtype = np.int16)
        plt.plot(arr1)
        plt.show()
        answer = simpledialog.askstring("Input", "Type path to file",
                                parent=top)
        if answer is not None:
            wavfile.write(answer , 16000 , arr1)
        top.destroy()
       



top.geometry("400x200")  
start = Button(top , font =("calibri" , 10) , text = "start", command = start_reading)
stop = Button(top , font =("calibri" , 10) , text = "stop", command = stop_reading)
quit = Button(top, font =("calibri" , 10) ,text="Quit", command=quit_app)





start.pack()
stop.pack()
quit.pack()


read_data()
  
top.mainloop() 

