#include <QApplication>
#include "baseconver.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    QApplication app(argc, argv);

    BaseConver bc;

    bc.show();

    return app.exec();
}
