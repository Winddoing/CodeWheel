#include "tcpserver.h"

#include <QTcpSocket>

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
}

void TcpServer::incomingConnection(int handle)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(handle);

    emit conectionConnected(socket);
}
