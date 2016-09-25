__author__ = 'WangQuan'


import socket
import sys

#creat
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

except socket.error ,msg :
    print "socket fail" + msg
    sys.exit()

print "creat socket %s" %s

#gethos ip
host = 'www.oschina.net'

try:
    ip = socket.gethostbyname(host)

except socket.gaierror :
    print "get host ip fail"
    sys.exit()

print "baidu ip:" + str(ip)

#set part
# part = 80
ip = '127.0.0.1'
part = 2222

#connect
try:
    s.connect((ip, part))
except socket.error, msg:
    print "connect fail, error code: " + str(msg[0])
    sys.exit()

print 'connect' + ip + 'part' + str(part)

#send
message = "GET / HTTP/1.1\r\n\r\n"

try:
    s.sendall(message)
except socket.error :
    print "send fail" + message
    sys.exit()

print "send success" + message

#receive
while True:
    reply = s.recv(1024)
    if not reply :
        print "\n\n...OK!!!"
        s.close()
        sys.exit()



print "receive data"
print reply

s.close()

