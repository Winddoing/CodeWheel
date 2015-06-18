#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <tcpserver.h>

class QTcpSocket;

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private:
    Ui::ConnectDialog *ui;

protected:
    void timerEvent(QTimerEvent *e);

signals:
    void gameStart(QTcpSocket *s, const QString &n,
                   const QPixmap *map, bool isServer);

private slots:
    void on_previewBtn_clicked();
    void incomingClient(QTcpSocket *s);
    void connectedToServer();
    void enableStartBtnSlot();
    void on_startBtn_clicked();

private:
    int timerId;
    TcpServer *server;
    QTcpSocket *socket;
};

#endif // CONNECTDIALOG_H
