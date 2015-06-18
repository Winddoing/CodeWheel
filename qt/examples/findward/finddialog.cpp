#include "finddialog.h"
#include "ui_finddialog.h"

findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)  //实例化ui
{
    ui->setupUi(this);  //实例化部件，构造设计器中的
//    connect(ui->lineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enableFindButton(QString)));
}
//void findDialog::enableFindButton(const QString &s)
//{
//    ui->findButton->setEnabled(! s.isEmpty());
//}
findDialog::~findDialog()
{
    delete ui;    //调用ui的析构函数
}


void findDialog::on_lineEdit_textChanged(const QString &s)
{
    ui->findButton->setEnabled(! s.isEmpty());
}

void findDialog::on_findButton_clicked()
{
   if(ui->backcheckBox->isChecked())
       emit findPrevSignal(ui->lineEdit->text(),
                           ui->casecheckBox->isChecked());
   else
       emit findNextSignal(ui->lineEdit->text(),
                           ui->casecheckBox->isChecked());
}
