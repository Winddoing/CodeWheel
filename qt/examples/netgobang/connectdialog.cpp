#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <tcpserver.h>
#include <QTimerEvent>
#include <QTcpSocket>
#include <QFileDialog>

#include <netgobang.h>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    ui->serverRadioBtn->setChecked(true);
    /*添加默认值*/
    ui->userEdit->setText(tr("张三"));

    connect(ui->serverRadioBtn, SIGNAL(toggled(bool)),
            this, SLOT(enableStartBtnSlot()));
    connect(ui->userEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableStartBtnSlot()));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::incomingClient(QTcpSocket *s)
{   // 发送信号套接子，用户名，头像、是否服务器连接
    emit this->gameStart(s, ui->userEdit->text(),
                         ui->headEdit->pixmap(), true);
    accept(); //以接受方式关闭窗口
}

void ConnectDialog::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == timerId)
    {
        ui->progressBar->setValue(
                (ui->progressBar->value() + 1) % 101);
    }
}

void ConnectDialog::on_startBtn_clicked()
{
    //是服务器
    if(ui->serverRadioBtn->isChecked())
    {
        //实例化TcpSer对象
        server = new TcpServer(this);
        //连接到槽
        connect(server, SIGNAL(conectionConnected(QTcpSocket*)),
                this, SLOT(incomingClient(QTcpSocket*)));
        //开启监听
        server->listen(QHostAddress::Any, ui->portPinBox->value());
    }else //是客户端
    {
        //实例化对象
        socket = new QTcpSocket;
        //连接到槽－发送
        connect(socket, SIGNAL(connected()),
                this, SLOT(connectedToServer()));
        //连接到服务器
        socket->connectToHost(QHostAddress(ui->ipEdit->text()),
                              ui->portPinBox->value());
    }

    //开启定时器
    timerId = startTimer(100);
    //禁用用户配置
    ui->netGroupBox->setEnabled(false);
    ui->ipGroupBox->setEnabled(false);
    ui->userGroupBox->setEnabled(false);
    ui->startBtn->setEnabled(false);

}

void ConnectDialog::connectedToServer()
{
    emit this->gameStart(socket, ui->userEdit->text(),
                         ui->headEdit->pixmap(), false);
    accept();
}

void ConnectDialog::enableStartBtnSlot()
{
    if(ui->userEdit->text().simplified().isEmpty())
        goto EanbleFalse;
    if(0 == ui->headEdit->pixmap())
        goto EanbleFalse;
    if(ui->ClientRadioBtn->isChecked())
    {
        QHostAddress addr(ui->ipEdit->text());
        if(addr.isNull())
             goto EanbleFalse;
    }
    ui->startBtn->setEnabled(true);
    return;
 EanbleFalse:
    ui->startBtn->setEnabled(false);
}

void ConnectDialog::on_previewBtn_clicked()
{
    QString s;
    s = QFileDialog::getOpenFileName(this, tr("打开图片"),
                                     ".", tr("ICONS(*.png *.jpg)"));
    if(s.isEmpty()) return;
    QPixmap map(s);
    if(map.isNull()) return;

    ui->headEdit->setPixmap(map.scaled(ui->headEdit->size()));

    enableStartBtnSlot();

}

