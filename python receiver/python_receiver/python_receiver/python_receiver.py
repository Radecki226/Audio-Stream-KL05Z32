import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import struct
from scipy.io import wavfile
from tkinter import *  
from tkinter import messagebox
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
s = serial.Serial('COM5' , baudrate = 230400)
messagebox.showinfo("Hello!" , "Port initialized!")

def read_data():
    global s
    global arr
    global temp_arr
    global p 
    global stream
    global counter
    if(run):
        sound = s.read(buff_size)
        arr.append(sound)
        stream.write(sound)
        #sound = s.read(buff_size)
        #counter += len(sound)
        #temp_arr+=sound
        #if(counter > 1023):
            #arr.append(temp_arr)
            #stream.write(bytes(temp_arr))
            #temp_arr = []
            #counter = 0;
        
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
    
    s.close()
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
        wavfile.write('sound.wav' , 16000 , arr1)
        top.destroy()
        time.sleep(2)
        playsound('sound.wav')



top.geometry("400x200")  
start = Button(top , font =("calibri" , 10) , text = "start", command = start_reading)
stop = Button(top , font =("calibri" , 10) , text = "stop", command = stop_reading)
quit = Button(top, font =("calibri" , 10) ,text="Quit", command=quit_app)





start.pack()
stop.pack()
quit.pack()


read_data()
  
top.mainloop() 

