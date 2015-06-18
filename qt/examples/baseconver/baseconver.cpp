#include "baseconver.h"

#include <QLineEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QString>
#include <QMouseEvent>


BaseConver::BaseConver(QWidget *parent) :
    QWidget(parent)
{
    oldBase = 16;
    converted = false;
    //初始化网格布局对象指针
    QGridLayout *gridLayout = new QGridLayout; //布局不能实例化为对象，
    lineEdit = new QLineEdit;
    //对行编辑器的对齐方式、固定高、字体等设置
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setFixedHeight(50);
    lineEdit->setStyleSheet("font-size:30px");
    //对lineEdit进行网格布局，     坐标(0,0)跨1行 5列
    gridLayout->addWidget(lineEdit, 0, 0, 1, 5);
    //通过循环添加数字按键
    for(int i = 0; i < 16; i++)
    {
        QPushButton *btn = new QPushButton(QString::number(i, 16).toUpper());
        //安装事件过滤器
        btn->installEventFilter(this);
        btn->setFixedSize(50, 50);
        btns.push_back(btn);
        //按键布局
        gridLayout->addWidget(btns[i], i / 4 + 1,
                             i % 4, 1, 1);
    }

    groupBox = new QGroupBox(tr("进制"));
    groupBox->setFixedWidth(75);

    QVBoxLayout *vLayout = new QVBoxLayout;
    binBtn = new QRadioButton("Bin");
    octBtn = new QRadioButton("Oct");
    decBtn = new QRadioButton("Dec");
    hexBtn = new QRadioButton("Hex");
    vLayout->addWidget(binBtn);
    vLayout->addWidget(octBtn);
    vLayout->addWidget(decBtn);
    vLayout->addWidget(hexBtn);
    groupBox->setLayout(vLayout);
    gridLayout->addWidget(groupBox, 1, 4, 3, 1); //先实例化，后布局

    backBtn = new QPushButton("<-");
    backBtn->setFixedSize(75, 50);
    gridLayout->addWidget(backBtn, 4, 4 ,1, 1);

    setFixedSize(sizeHint());
    setLayout(gridLayout);
    setWindowTitle(tr("进制转换器"));


    connect(binBtn, SIGNAL(toggled(bool)),
            this, SLOT(binBtnToggled(bool)));
    connect(octBtn, SIGNAL(toggled(bool)),
            this, SLOT(octBtnToggled(bool)));
    connect(decBtn, SIGNAL(toggled(bool)),
            this, SLOT(decBtnToggled(bool)));
    connect(hexBtn, SIGNAL(toggled(bool)),
            this, SLOT(hexBtnToggled(bool)));
    connect(backBtn, SIGNAL(clicked()),
            this, SLOT(backClicked()));
    //初始化单选框
    decBtn->setChecked(true);
}



void BaseConver::changeBaseTo(int newBase)
{
    int min = newBase < oldBase ? newBase : oldBase;
    int max = newBase > oldBase ? newBase : oldBase;

    for(; min < max; min++)
    {
        btns[min]->setEnabled(oldBase <= newBase);
    }

    bool ok;
    //将文本框中的字符串转换成当前的oldBase进制数
    qulonglong n = lineEdit->text().toULongLong(&ok, oldBase);
    //将转换后的新进制，输出
    lineEdit->setText(QString::number(n , newBase).toUpper());

    oldBase = newBase;   //!!!!相当于重新初始化oldbase为下一次转换准备
    converted = true;
}

void BaseConver::backClicked()
{
    bool ok;
    qulonglong n = 0;
    if(!converted)
        //一个数减少一位，是对它以它的进制数去整
        //一个书增加一位，是对他乘以它的进制数
       n = lineEdit->text().toULongLong(&ok, oldBase) / oldBase;

    lineEdit->setText(QString::number(n , oldBase).toUpper());

}

bool BaseConver::eventFilter(QObject *obj, QEvent *event)
{
    QPushButton *btn = (QPushButton *)obj;
    QMouseEvent *e = (QMouseEvent *)event;
    //判断哪个数字键被点击
    if(-1 == btns.indexOf(btn))
        return QWidget::eventFilter(obj, event);
    //判断事件类型，是否为鼠标点击事件
    if(e->type() != QMouseEvent::MouseButtonPress)
        return QWidget::eventFilter(obj, event);
    //判断是否为鼠标左键点击
    if(e->button() != Qt::LeftButton)
        return QWidget::eventFilter(obj, event);
    bool ok;
    qulonglong n = 0;
    if(!converted)
         n = lineEdit->text().toULongLong(&ok, oldBase) * oldBase;

    n += btn->text().toULongLong(&ok, oldBase);
    lineEdit->setText(QString::number(n , oldBase).toUpper());

    converted = false;

    return QWidget::eventFilter(obj, event);
}

void BaseConver::binBtnToggled(bool checked)
{
    if(checked) changeBaseTo(2);
}

void BaseConver::octBtnToggled(bool checked)
{
    if(checked) changeBaseTo(8);
}
void BaseConver::decBtnToggled(bool checked)
{
    if(checked) changeBaseTo(10);
}
void BaseConver::hexBtnToggled(bool checked)
{
    if(checked) changeBaseTo(16);
}
