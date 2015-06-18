#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
    class findDialog;
}

class findDialog : public QDialog
{
    Q_OBJECT

public:
    explicit findDialog(QWidget *parent = 0);
    ~findDialog();

//private slots:
//    void enableFindButton(const QString &s);

private:
    Ui::findDialog *ui;
signals:
    void findPrevSignal(const QString &s, bool ignCase);
    void findNextSignal(const QString &s, bool ignCase);


private slots:
    void on_findButton_clicked();
    void on_lineEdit_textChanged(const QString &);

};

#endif // FINDDIALOG_H
