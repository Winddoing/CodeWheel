#include <QtGui/QApplication>
#include "pictureview.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(
            QTextCodec::codecForName("utf8"));
    QApplication a(argc, argv);
    PictureView w;
    w.show();

    return a.exec();
}
