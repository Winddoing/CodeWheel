#include "pictureview.h"
#include "ui_pictureview.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include <QPixmap>
#include <QMatrix>

PictureView::PictureView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PictureView)
{
    ui->setupUi(this);
    angle = 0;
    zoom = 100;
    fileName = "";
    map = 0;

    nameLabel = new QLabel(tr("未打开文件"));
    zoomComboBox = new QComboBox;
    for(int i = MinZoom; i <= MaxZoom; i += StepZoom)
    {
        //向下拉列表框控件中添加列表项
        //QString::number()可以将任意类型数值转换为QString
        //+是一个运算符重载，用于将const char *与QString连接
        zoomComboBox->addItem(QString::number(i) + "%");
    }
    //根据列表项索引值设置下拉列表框默认显示的列表项setCurrentIndex
    zoomComboBox->setCurrentIndex(
    //在所有列表项中做字符串查找来获取"100%"这一项的索引值
            zoomComboBox->findText("100%"));
    angleComboBox = new QComboBox;
    for(int i = MinAngle; i <= MaxAngle; i += StepAngle)
    {
        angleComboBox->addItem(QString::number(i)
                               + tr("度"));
    }
    angleComboBox->setCurrentIndex(0);

    //向statusBar上添加控件
    //1.statusBar是设计器画出来的，不是手动添加的，所以它属于ui对象
    //2.statusBar能够从左向右自动排列控件，证明它一定有布局
    //           所以可知ui->statusBar->layout()可以取得其布局
    //3.可以改变其布局的对齐方式为右对齐
    ui->statusBar->layout()->setAlignment(Qt::AlignRight);
    //4.可以通过QStatusBar::addWidget函数向状态栏添加控件
    //  当然也可以向状态栏的布局中添加控件
    ui->statusBar->layout()->addWidget(nameLabel);
    ui->statusBar->layout()->addWidget(zoomComboBox);
    ui->statusBar->layout()->addWidget(angleComboBox);
}

PictureView::~PictureView()
{
    delete ui;
    //如果一个new出的控件,并且没有指定父窗口(即父窗口是桌面)
    //在析构中一定要将其delete，否则内存泄露
    if(map != 0)
    {
        delete map;
        map = 0;
    }
}

void PictureView::on_aboutUsAction_activated()
{
    QMessageBox::information(this, tr("关于我们"),
           tr("我也不知道写什么"), tr("确定"));
}

void PictureView::on_aboutQtAction_activated()
{
    QMessageBox::aboutQt(this, tr("关于QT"));
}

void PictureView::on_openAction_activated()
{
    //输入图片文件名称 QFileDialog::getOpenFileName
    QString s = QFileDialog::getOpenFileName(this,
                tr("打开图片"), ".",
                tr("IMG(*.jpg *.bmp *.png)"));
    if(s.isEmpty()) return;
    QPixmap *mapTmp = new QPixmap(s);
    if(mapTmp->isNull())
    {
        QMessageBox::warning(this, tr("文件打开失败"),
                             tr("无法打开:") + s,
                             tr("确定"));
        delete mapTmp;
        return;
    }
    fileName = s;
    zoom = DefZoom;
    angle = DefAngle;
    if(map != 0) delete map;
    map = mapTmp;
    showPixmap();
    //把图片读入系统内存 QImage QPixmap
    //利用控件将图片显示出来 QLable
}
void PictureView::showPixmap()
{
    if(0 == map) return;
    QPixmap mapTmp = map->scaled(map->width() * zoom / 100,
                              map->height() * zoom / 100);
    QMatrix m;  //实例化一个矩阵对象
    m.rotate(angle);    //将此矩阵按angle角度进行旋转
    mapTmp = mapTmp.transformed(m);//让图片按矩阵进行变换

    nameLabel->setText(fileName);
    zoomComboBox->setCurrentIndex(
            zoomComboBox->findText(
                    QString::number(zoom) + "%"));
    angleComboBox->setCurrentIndex(
            angleComboBox->findText(
                    QString::number(angle) + tr("度")));

    ui->imgLabel->setPixmap(mapTmp);
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
    angle = DefAngle;
    zoom = DefZoom;
    showPixmap();
}

void PictureView::on_closeAction_activated()
{
    if(map != 0)
    {
        delete map;
        map = 0;
    }
    zoom = DefZoom;
    angle = DefAngle;
    nameLabel->setText(tr("未打开文件"));
    zoomComboBox->setCurrentIndex(
            zoomComboBox->findText(
                    QString::number(DefZoom) + "%"));
    angleComboBox->setCurrentIndex(
            angleComboBox->findText(
                    QString::number(DefAngle) + tr("度")));
    ui->imgLabel->setText(" ");
}
