#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    enum Board{Rows = 20, Cols = 20,
               Width = 24, Height = 24};
    explicit ChessBoard(QWidget *parent = 0);
    QSize sizeHint() const;

signals:
    void localChess(const QPoint &p);

public slots:
    void remoteChess(const QPoint &p);
    void start();

private:
    //根据当前落子的点来确定是本方还是对方获胜
    bool isVictory(bool local, const QPoint &p);
    bool isVicroryDirect(const QVector<QPoint> &points,
                         const QPoint &p, int offX, int offY);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);

private:
    QPixmap localMap;
    QPixmap remoteMap;
    QVector<QPoint> localPoints;
    QVector<QPoint> remotePoints;
    bool islocal;

};

#endif // CHESSBOARD_H
