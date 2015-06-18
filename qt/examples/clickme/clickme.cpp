#include "clickme.h"
#include "ui_clickme.h"

#include <time.h>
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

ClickMe::ClickMe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClickMe)
{
    ui->setupUi(this);
    setFixedSize(sizeHint());
    map.load(":/icons/clickme.png");
    srand(time(0));
}

ClickMe::~ClickMe()
{
    delete ui;
}

void ClickMe::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.drawPixmap(frameArea, map);

    return QWidget::paintEvent(e);
}

void ClickMe::setRect()
{
    frameArea.setX(rand() % (ui->frame->width() - map.width()) +
                   ui->frame->geometry().x());
    frameArea.setY(rand() % (ui->frame->height() - map.height()) +
                   ui->frame->geometry().y());
    frameArea.setSize(map.size());
}

void ClickMe::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton ||
       0 == ui->secondLcdNumber->intValue())
        return QWidget::mousePressEvent(e);
    if(frameArea.contains(e->pos()))
    {
        ui->timersLcdNumber->display(ui->timersLcdNumber->intValue() + 1);
        setRect();
        update();
    }
    return QWidget::mousePressEvent(e);
}

void ClickMe::timerEvent(QTimerEvent *e)
{
    if(e->timerId() != timerId)
        return QWidget::timerEvent(e); //如果当前定时器ID与响应事件的定时器ID不一致，则交由父类函数处理
    int sec = ui->secondLcdNumber->intValue() - 1;

    ui->secondLcdNumber->display(sec);
    if(0 == sec)
    {
        killTimer(timerId);
        ui->startPushButton->setEnabled(true);
    }
    return QWidget::timerEvent(e);
}

void ClickMe::on_startPushButton_clicked()
{
    ui->secondLcdNumber->display(30);
    ui->timersLcdNumber->display(0);
    ui->startPushButton->setEnabled(false);
    timerId = startTimer(1000);
    update();
    setRect();
}
