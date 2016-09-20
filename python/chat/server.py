__author__ = 'WangQuan'


import socket,select

def broadcast_data (sock,message):
    for socket in conn_list:
        if socket != server_socket and socket != sock :
            try :
                socket.send(message)
            except :
                socket.close()
                conn_list.remove(socket)

if __name__ == "__main__":
    conn_list = []
    recv_buffer = 4096
    PORT = 22222

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_socket.bind(('', PORT))
    server_socket.listen(99)

    conn_list.append(server_socket)

    for x in conn_list :
        print "wqshao:" + str(x)

    print "Chat server started on port " + str(PORT)

    while 1:
        read_sockets,write_sockets,error_sockets = select.select(conn_list,[],[])
        for sock in read_sockets:

            if sock == server_socket:
                sockfd, addr = server_socket.accept()
                conn_list.append(sockfd)
                print "Client (%s, %s) connected" % addr
                broadcast_data(sockfd, "[%s:%s] entered room\n" % addr)

            else:
                try:
                    data = sock.recv(recv_buffer)
                    if data:
                        broadcast_data(sock, "\r" + '<' + str(sock.getpeername()) + '> ' + data)
                except:
                    broadcast_data(sock, "Client (%s, %s) is offline" % addr)
                    print "Client (%s, %s) is offline" % addr
                    sock.close()
                    conn_list.remove(sock)
                continue

    server_socket.close()