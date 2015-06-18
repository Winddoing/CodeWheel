#ifndef BASECONVER_H
#define BASECONVER_H

#include <QWidget>
#include <QVector>

class QLineEdit;
class QPushButton;
class QGroupBox;
class QRadioButton;

class BaseConver : public QWidget
{
    Q_OBJECT
public:
    explicit BaseConver(QWidget *parent = 0);

private slots:
    void binBtnToggled(bool checked);
    void octBtnToggled(bool checked);
    void decBtnToggled(bool checked);
    void hexBtnToggled(bool checked);
    void backClicked();

private:
    void changeBaseTo(int newBase);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QLineEdit *lineEdit;
    QGroupBox *groupBox;
    QRadioButton  *binBtn,
                *octBtn,
                *decBtn,
                *hexBtn;
    QPushButton *backBtn;
    QVector<QPushButton *> btns;
    int oldBase;
    bool converted;

};

#endif // BASECONVER_H
