#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

namespace Ui {
    class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    void newFile();     //新建
    bool maybeSave();   //是否保存
    bool save();        //保存
    bool saveAs();      //另存为
    bool saveFile(const QString &fileName);  //保存文件
    ~Notepad();

private:
    Ui::Notepad *ui;
    bool isSave;
    // 保存当前文件的路径
    QString curFile;

public slots:
    void newBuild_triggered();
    void save_triggered();
    void saveAs_triggered();

};

#endif // NOTEPAD_H
