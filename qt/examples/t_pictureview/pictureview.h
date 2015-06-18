#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QMainWindow>
class QPixmap;
//QPixmap   用于图片的显示
//QImage    用于图片的编辑
//QBitMap   用于黑白二值图片，常用于遮罩

class QLabel;
class QComboBox;

namespace Ui {
    class PictureView;
}

class PictureView : public QMainWindow
{
    Q_OBJECT

public:
    explicit PictureView(QWidget *parent = 0);
    ~PictureView();
    enum ZoomRange{MinZoom = 20,
                   DefZoom = 100,
                   MaxZoom = 200,
                   StepZoom = 20};
    enum AngleRange {MinAngle = 0,
                     DefAngle = 0,
                     MaxAngle = 270,
                     StepAngle = 90};
private:
    Ui::PictureView *ui;

private slots:
    void on_closeAction_activated();
    void on_resumeAction_activated();
    void on_rotateAction_activated();
    void on_zoominAction_activated();
    void on_zoomoutAction_activated();
    void on_openAction_activated();
    void on_aboutQtAction_activated();
    void on_aboutUsAction_activated();
private:
    void showPixmap();
private:
    QLabel *nameLabel;
    QComboBox *zoomComboBox;
    QComboBox *angleComboBox;
    QString fileName;
    int zoom;
    int angle;
    QPixmap *map;
};

#endif // PICTUREVIEW_H
