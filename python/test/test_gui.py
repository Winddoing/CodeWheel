# -*- coding: utf-8 -*-



from Tkinter import *
import ttk
# import Pmw
import time        
import re

import struct
import socket
import ctypes as ct

import Queue
import thread    
class Server(object):
    def __init__(self,parent):
        
        self.parent = parent
        
        panedwindow = PanedWindow(self.parent,bg = 'green',orient = VERTICAL)
        frame = Frame(self.parent)
        panedwindow.pack(fill = BOTH,expand = 1)
        
        Label(frame,text = '聊天记录').pack(fill = Y,expand = 0)
        
        self.record =Text(frame,bg = '#F0FFFF',font = ('KaiTi',15),height = 10,width= 10)
        self.record.bind('<KeyPress>',lambda e:'break')
        self.scroll = Scrollbar(frame,command = self.record.yview)
        self.record.config(yscrollcommand = self.scroll.set)
        self.record.pack(side = LEFT,fill = BOTH,expand = 1)
        self.scroll.pack(side = RIGHT,fill = Y,expand = 0)
        
        panedwindow.add(frame)
        
        
        frame = Frame(self.parent)
        frame.pack(fill = BOTH,expand = 1)
        
        Label(frame,text = '输入内容').pack(fill = Y,expand = 0)
        
        self.sendtext =Text(frame,bg = '#FAFFF0',font = ('KaiTi',15),height = 10,width= 10)
        scroll = Scrollbar(frame,command = self.sendtext.yview)
        self.sendtext.config(yscrollcommand = scroll.set)
        self.sendtext.pack(side = LEFT,fill = BOTH,expand = 1)
        scroll.pack(side = RIGHT,fill = Y,expand = 0)
        
        panedwindow.add(frame)
        
        
        frameBottom = Frame(self.parent)
        frameBottom.pack(side = BOTTOM,fill=  X,expand = 0)
        frame = Frame(frameBottom)
        Label(frame,text = '选择IP:').pack(side = LEFT)
        self.entry = Entry(frame)
        self.entry.pack(side = LEFT)
        frame.pack(side = LEFT)
        ttk.Button(frameBottom,text = '发送',command = self.sendMessage).pack(side = RIGHT)
        
        
        self.time = ShowtimeLabel()
        self.server = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.server.bind(('localhost',8000))
        self.queue = Queue.Queue(10)
        
        self.state = 0
        self.search()
        '''开一个线程用来接收网口数据,接收到的数据送到队列中'''
        
    def search(self):
        
        if self.state == 0:
            thread.start_new_thread(self.serverOperate,())
        if self.queue.empty() ==True:
            self.state = 1
            self.parent.after(200,self.search)
       
        else:
            self.state = 0
  
            message = self.queue.get_nowait()
            print message,self.time.data
#            hour = re.compile(r'\s(\d*)\s:')
#            time = re.findall(hour,self.time.data)
#            time[0]
            
            self.record.insert(END,self.time.data+'\n')
            self.record.insert(END,message[1]+" :"+message[0])
            self.record.yview_moveto(1)
            self.parent.after(200,self.search)
          
    def serverOperate(self):
        
        data,(addr,port) = self.server.recvfrom(1024)
        message =  [data,addr,port]    
        self.queue.put(message)
        
        
    def sendMessage(self):
        sendToIp = self.entry.get()
        if sendToIp == '':
            sendToIp = 'localhost'
        
            
        client = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        message = self.sendtext.get(1.0,END)
        self.sendtext.delete(1.0,END)
        
        client.sendto(message,(sendToIp,8000))
        client.close()
        
        
'''以前做的用来显示时间的模块'''    
class ShowtimeLabel(Label):     #创建一个用来显示时间的表
    def __init__(self, *args, **kwargs):
        
        Label.__init__(self, *args, **kwargs)
        self.config(wraplength = 90,relief = SUNKEN,fg = 'white')
        week = StringVar()
        week = self.Week
        self.balloon = Pmw.Balloon(self)
        self.balloon.bind(self,week)
        self.show()
    @property
    def data(self):
        return time.strftime(' %H : %M : %S  %Y / %m / %d   ',time.localtime(time.time())) 
    @property
    def Week(self):
        return time.strftime('%A',time.localtime(time.time()))
    
    def show(self):
        self.time = StringVar()
        self.time.set(self.data)
        
        self.config(textvariable = self.time)

        self.after(100,self.show)           
if __name__ == "__main__":
    root = Tk()
    serv = Server(root)
    #root.geometry('300x800')
    root.mainloop()