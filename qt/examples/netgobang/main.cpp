#include <QApplication>
#include <QTextCodec>

#include <connectdialog.h>
#include <netgobang.h>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QApplication app(argc, argv);

    ConnectDialog cd;
    NetGoBang nb;

    QObject::connect(&cd, SIGNAL(gameStart(QTcpSocket*,QString,const QPixmap*,bool)),
                     &nb, SLOT(startGame(QTcpSocket*,QString,const QPixmap*,bool)));
    if(QDialog::Rejected == cd.exec())
        return 0;
    nb.show();

    return app.exec();
}
