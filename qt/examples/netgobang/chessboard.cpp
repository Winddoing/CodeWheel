#include "chessboard.h"

#include <QPainter>
#include <QVector>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include <QMessageBox>

ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent)
{
    islocal = true;
    localMap.load(":/icons/local.png");
    remoteMap.load(":/icons/remote.png");
    setFixedSize(sizeHint());
}

QSize ChessBoard::sizeHint() const
{
    return QSize(Cols * Width, Rows * Height);
}

void ChessBoard::start()
{
    localPoints.empty();
    remotePoints.empty();
    update();
    islocal = true;
}

void ChessBoard::mousePressEvent(QMouseEvent *e)
{//
    if(!islocal || e->button() != Qt::LeftButton)
        return QWidget::mousePressEvent(e);
    QPoint p;
    p.setX(e->pos().x() / Width);
    p.setY(e->pos().y() / Height);
    //坐标是否超出棋盘范围
    if(p.x() >= Width || p.y() >= Height)
        return QWidget::mousePressEvent(e);
    //此坐标上是否有棋子
    if(localPoints.indexOf(p) != -1 ||
       remotePoints.indexOf(p) != -1)
        return QWidget::mousePressEvent(e);
    localPoints.append(p);
    islocal = false;
    emit localChess(p);
    update();
    if(isVictory(true, p))
    {
        QMessageBox::information(this, tr("恭喜你"), tr("你赢了"));
    }

    return QWidget::mousePressEvent(e);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(Qt::black);

    for(int i = 0; i < Cols; i++)
    {
        painter.drawLine(Width / 2 + i * Width, Height / 2,
                         Width / 2 + i * Width,
                         -Height / 2 + Rows * Height);
    }

    for(int i = 0; i < Rows; i++)
    {
        painter.drawLine(Width / 2, Height / 2 + i * Height,
                        - Width / 2 + Cols * Width,
                        Height / 2 + i* Height);
    }

    for(int i = 0; i < localPoints.count(); i++)
    {
        painter.drawPixmap(localPoints[i].x() * Width + 3,
                           localPoints[i].y() * Height + 3,
                           localMap.width(),
                           localMap.height(),
                           localMap);
    }

    QVector<QPoint>::iterator it;
    it = remotePoints.begin();
    while(it != remotePoints.end())
    {
        painter.drawPixmap((*it).x() * Width + 3,
                           (*it).y() * Height + 3,
                           remoteMap.width(),
                           remoteMap.height(),
                           remoteMap);
        ++it;
    }

}

void ChessBoard::remoteChess(const QPoint &p)
{
    islocal = true;
    remotePoints.append(p);
    update();
    if(isVictory(false, p))
    {
       islocal = false;
       QMessageBox::information(this, tr("对不起"), tr("你输了"));
    }
}

bool ChessBoard::isVictory(bool local, const QPoint &p)
{
    const QVector<QPoint> &points =
            local ? localPoints : remotePoints;
    if(isVicroryDirect(points, p, 1, 0))return true;
    if(isVicroryDirect(points, p, 0, 1))return true;
    if(isVicroryDirect(points, p, 1, 1))return true;
    if(isVicroryDirect(points, p, 1, -1))return true;

    return false;
}

bool ChessBoard::isVicroryDirect(const QVector<QPoint> &points,
                        const QPoint &p, int offX, int offY)
{
    int count = 1;
    QPoint tmp;

    for(int i = 1; i < 5; i++)
    {
        tmp = QPoint(p.x() + offX * i,
                     p.y() + offY * i);
        if(points.indexOf(tmp) == -1) break;
        count++;
    }
    if(count >= 5) return true;
    for(int i = 1; i < 5; i++)
    {
        tmp = QPoint(p.x() - offX * i,
                     p.y() - offY * i);
        if(points.indexOf(tmp) == -1) break;
        count++;
    }
    return (count >= 5);
}


