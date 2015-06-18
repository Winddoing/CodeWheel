#ifndef CLICKME_H
#define CLICKME_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QRect>

namespace Ui {
    class ClickMe;
}

class ClickMe : public QWidget
{
    Q_OBJECT

public:
    explicit ClickMe(QWidget *parent = 0);
    ~ClickMe();

private:
    Ui::ClickMe *ui;
    int timerId;
    QPixmap map;
    QRect frameArea;

private:
    void setRect();

protected:
    void timerEvent(QTimerEvent *e); //定时器事件处理函数
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_startPushButton_clicked();

};

#endif // CLICKME_H
