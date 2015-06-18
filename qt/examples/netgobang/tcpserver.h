#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

protected:
    void incomingConnection(int handle);

signals:
    void conectionConnected(QTcpSocket *s);
};

#endif // TCPSERVER_H
