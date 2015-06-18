#include "netgobang.h"
#include "ui_netgobang.h"

#include <chessboard.h>
#include <QHBoxLayout>
#include <QVariant>
#include <QDataStream>
#include <QTcpSocket>
#include <QString>
#include <QPixmap>
#include <QMessageBox>
#include <QAbstractSocket>

NetGoBang::NetGoBang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetGoBang)
{
//    socket = 0;
    blockSize = 0;
    ui->setupUi(this);
    chessBoard = new ChessBoard;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(chessBoard);
    ui->frame->setLayout(hLayout);
    setFixedSize(sizeHint());

    connect(chessBoard, SIGNAL(localChess(QPoint)),
            this, SLOT(loaclChess(QPoint)));
}

NetGoBang::~NetGoBang()
{
    delete ui;
}

void NetGoBang::startGame(QTcpSocket *s, const QString &n,
               const QPixmap *map, bool isServer)
{
    socket = s;
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readData()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(AbstractSocket::SocketError)));
    ui->localHeadLabel->setPixmap(*map);
    ui->localUserEdit->setText(n);
    QByteArray by;
    QDataStream in(&by, QIODevice::WriteOnly);
    in << n << *map;
    sendData(by);
    if(isServer)
    {
        chessBoard->start();
        ui->currentNameLabel->setText(tr("本方"));
    }else{
        ui->currentNameLabel->setText(tr("对方"));
    }
}

void NetGoBang::sendData(const QVariant &data)
{
    QByteArray by;
    QDataStream io(&by, QIODevice::ReadWrite);
    quint8 type;
    switch((int)(data.type()))  //switch默认变量为整型
    {
    case QVariant::ByteArray:
            type = UserInfo;
            by = data.toByteArray();
            break;
    case QVariant::String:
            type = ChatString;
            io << data.toString();
            break;
    case QVariant::Point:
            type = ChessPos;
            io << data.toPoint();
            break;
//    default:
//            break;
    }

    quint16 length = by.size() + sizeof(type);

    QDataStream out(socket);
    out << length << type << by;
}

void NetGoBang::readData()
{
    QDataStream out(socket);
    if(0 == blockSize)
        out >> blockSize ;
    if(socket->bytesAvailable() < blockSize)
        return;
    QByteArray by;
    quint8 type;
    out >> type >> by;

    QDataStream io(by);
    QString s;
    QPoint p;

    switch(type)
    {
    case ChatString:
        io >> s;
        ui->textEdit->append(
                ui->remoteUserEdit->text() + tr("说：") + s + tr("</font>"));
        break;
    case ChessPos:
        io >> p;
        ui->currentNameLabel->setText(tr("我方"));
        chessBoard->remoteChess(p);
        break;
    case UserInfo:
        QPixmap map;
        QString name;
        io >> name >> map;
        ui->remoteHeadLabel->setPixmap(map);
        ui->remoteUserEdit->setText(name);
        break;
    }

    blockSize = 0;
}

void NetGoBang::loaclChess(const QPoint &p)
{
    sendData(p);
    ui->currentNameLabel->setText(tr("对方"));
}

void NetGoBang::on_sendLineEdit_textChanged(QString s)
{
    ui->sendBtn->setEnabled(!s.isEmpty());
}

void NetGoBang::on_sendBtn_clicked()
{
    sendData(ui->sendLineEdit->text());
    ui->textEdit->append(tr("我说：") + ui->sendLineEdit->text());
    ui->sendLineEdit->clear();
}

void NetGoBang::socketError(QAbstractSocket::SocketError)
{
    QMessageBox::critical(this, tr("网络出错"), socket->errorString(), tr("确定"));
}
