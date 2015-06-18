#include <QtGui/QApplication>
#include "clickme.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClickMe w;
    w.show();

    return a.exec();
}
