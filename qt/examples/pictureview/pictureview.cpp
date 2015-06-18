#include "pictureview.h"
#include "ui_pictureview.h"

#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QMatrix>
#include <QLabel>
#include <QComboBox>
#include <QLayout>

PictureView::PictureView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PictureView)
{
    ui->setupUi(this);

    fileName = "";
    zoom = DefZoom;
    angle = DefAngle;
    map = 0;

    nameLabel = new QLabel(tr("未打开文件"));
    zoomComboBox = new QComboBox;
    for(int i = MinZoom; i <= MaxZoom; i += StepZoom)
    {
        zoomComboBox->addItem(QString::number(i) + "%");
    }
    zoomComboBox->setCurrentIndex(zoomComboBox->findText("100%"));

    angleComboBox = new QComboBox;
    for(int i = MinAngle; i <= MaxAngle; i += StepAngle)
    {
        angleComboBox->addItem(QString::number(i) + tr("度"));
    }
    angleComboBox->setCurrentIndex(angleComboBox->findText(tr("0度")));

    //ui->statusBar->layout()->setAlignment(Qt::AlignRight);
    ui->statusBar->addWidget(nameLabel);
    ui->statusBar->addWidget(zoomComboBox);
    ui->statusBar->addWidget(angleComboBox);

}

PictureView::~PictureView()
{
    delete ui;
}

void PictureView::on_aboutQtAction_activated()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}

void PictureView::on_openAction_activated()
{
    QString s = QFileDialog::getOpenFileName(this, tr("打开图片"),
                                            ".", tr("IMG(*.jpg *.png *.bmp)"));
    if(s.isEmpty()) return;
    QPixmap *mapTmp;

    mapTmp = new QPixmap(s);
    if(mapTmp->isNull())
    {
        QMessageBox::warning(this, tr("打开文件失败"),
                             tr("文件打开失败") + s, tr("确定"));
        delete mapTmp;
        mapTmp = 0;
    }
    fileName = s;
    zoom = DefZoom;
    angle = DefAngle;
    if(map) delete map;
    map = mapTmp;
    showPixmap();
}

void PictureView::showPixmap()
{
    //判断当前是否有图片打开，如果没有不进行显示
    if(0 == map) return;
    ui->pictureLabel->setAlignment(Qt::AlignCenter);
    //对图片放缩（宽高按一定比例减小，增大）
    QPixmap mapTmp = map->scaled(map->width() * zoom / 100,
                                 map->height() * zoom /100);
    //图片旋转，矩阵变换
    QMatrix mapMatrix; //实例化一个矩阵对象
    //1.将mapMatrix矩阵对象，按angle角度进行旋转
    //2.将图片按矩阵变换
    mapTmp = mapTmp.transformed(mapMatrix.rotate(angle));

    nameLabel->setText(fileName);
    zoomComboBox->setCurrentIndex(zoomComboBox->findText(QString::number(zoom) + "%"));
    angleComboBox->setCurrentIndex(angleComboBox->findText(QString::number(angle) + tr("度")));
    ui->pictureLabel->setPixmap(mapTmp);
    adjustSize();
}

void PictureView::on_zoomoutAction_activated()
{
    zoom -= StepZoom;
    if(zoom < MinZoom) zoom = MinZoom;
    showPixmap();
}

void PictureView::on_zoominAction_activated()
{
    zoom += StepZoom;
    if(zoom > MaxZoom) zoom = MaxZoom;
    showPixmap();
}

void PictureView::on_rotateAction_activated()
{
    angle += StepAngle;
    if(angle > MaxAngle) angle = MinAngle;
    showPixmap();
}

void PictureView::on_resumeAction_activated()
{
    zoom = DefZoom;
    angle = DefAngle;
    showPixmap();
}

void PictureView::on_closeAction_activated()
{
    if(map)
    {
        delete map;
        map = 0;
    }
    ui->pictureLabel->setText(" ");
}

void PictureView::on_aboutUsAction_activated()
{
    QMessageBox::information(this, tr("关于我们"), tr("瓏创＠林风制作"), tr("确定"));
}
