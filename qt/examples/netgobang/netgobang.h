#ifndef NETGOBANG_H
#define NETGOBANG_H

#include <QWidget>
#include <QAbstractSocket>

class ChessBoard;
class QTcpSocket;

namespace Ui {
    class NetGoBang;
}

class NetGoBang : public QWidget
{
    Q_OBJECT

public:
    enum DataType{UserInfo = 1, ChatString, ChessPos};
    explicit NetGoBang(QWidget *parent = 0);
    ~NetGoBang();

public slots:
    void startGame(QTcpSocket *s, const QString &n,
                   const QPixmap *map, bool isServer);

private:
    void sendData(const QVariant &data);

private:
    Ui::NetGoBang *ui;
    ChessBoard *chessBoard;
    QTcpSocket *socket;
    quint16 blockSize;

private slots:
    void readData();
    void socketError(QAbstractSocket::SocketError );
    void loaclChess(const QPoint &p);
    void on_sendBtn_clicked();
    void on_sendLineEdit_textChanged(QString );
};

#endif // NETGOBANG_H
