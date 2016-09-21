__author__ = 'WangQuan'

import socket
import sys
# from thread import *
import thread
# HOST = "192.168.1.12"
HOST = ''
POART = 2222

#creat
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
    print "creat faill" + msg
print "creat success"

#bind
try:
    s.bind((HOST, POART))
except socket.error, msg:
    print "bind faill, Error Code: " + str(msg[0]) + " message: " + str(msg[1])
    sys.exit()
print "bind success"

#listen
s.listen(10)
print "start listen"

def clientthread(conn):
    conn.send("Welcome to the server!!!\n   --wqshao")
    while True:
        data = conn.recv(4096)
        reply = "OK.." + data
        if not data:
            break
        conn.sendall(reply)

    conn.close()
    thread.exit_thread()

while 1:
    #accept
    conn, addr = s.accept()
    print "Connect with: " + addr[0] + ":" + str(addr[1])

    thread.start_new_thread(clientthread, (conn,))

    #receiv
    # data = conn.recv(1024)
    # conn.sendall(data)
    #
    # conn.close()

s.close()

