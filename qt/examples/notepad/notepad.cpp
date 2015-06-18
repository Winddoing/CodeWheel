#include "notepad.h"
#include "ui_notepad.h"

#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>


Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    isSave = true;
    curFile = tr("未命名.txt");
    this->setWindowIconText(curFile);
    ui->setupUi(this);
    QObject::connect(ui->newBuild, SIGNAL(triggered()),
                     this, SLOT(newBuild_triggered()));
    QObject::connect(ui->save, SIGNAL(triggered()),
                     this, SLOT(save_triggered()));
    QObject::connect(ui->saveAs, SIGNAL(triggered()),
                     this, SLOT(saveAs_triggered()));
}

Notepad::~Notepad()
{
    delete ui;
}

bool Notepad::maybeSave()   //判断是否保存
{
    if (ui->textEdit->document()->isModified()) {
        // 自定义一个警告对话框
         QMessageBox box;
         box.setWindowTitle(tr("警告"));
         box.setIcon(QMessageBox::Warning);
         box.setText(curFile + tr(" 尚未保存，是否保存？"));
         QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                          QMessageBox::YesRole);
         box.addButton(tr("否(&N)"), QMessageBox::NoRole);
         QPushButton *cancelBut = box.addButton(tr("取消"),
                          QMessageBox::RejectRole);
         box.exec();
         if (box.clickedButton() == yesBtn)
              return save();
         else if (box.clickedButton() == cancelBut)
              return false;
     }
     // 如果文档没有被更改，则直接返回true
     return true;
}

void Notepad::newFile()     //新建
{
    if (maybeSave()) {
           isSave = true;
           curFile = tr("未命名.txt");
           setWindowTitle(curFile);
           ui->textEdit->clear();
           ui->textEdit->setVisible(true);
       }
}

bool Notepad::save()        //保存
{
    if (isSave) {
           return saveAs();
       } else {
           return saveFile(curFile);
       }
}
bool Notepad::saveAs()      //另存为
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                             tr("另存为"),curFile);
       if (fileName.isEmpty()) return false;
       return saveFile(fileName);
}
bool Notepad::saveFile(const QString &fileName)  //保存文件
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
       // %1和%2分别对应后面arg两个参数，/n起换行的作用
       QMessageBox::warning(this, tr("多文档编辑器"),
                   tr("无法写入文件 %1：/n %2")
                  .arg(fileName).arg(file.errorString()));
       return false;
    }
    QTextStream out(&file);
    // 鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    // 鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    isSave = false;
    // 获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}
void Notepad::newBuild_triggered()
{
   newFile();
}
void Notepad::save_triggered()
{
   save();
}
void Notepad::saveAs_triggered()
{
   saveAs();
}
