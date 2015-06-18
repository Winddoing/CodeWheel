#include <QtGui/QApplication>
#include "notepad.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QApplication a(argc, argv);
    Notepad w;
    w.show();

    return a.exec();
}
