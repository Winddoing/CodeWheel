#include <QtGui/QApplication>
#include "finddialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    findDialog w;
    w.show();

    return a.exec();
}
