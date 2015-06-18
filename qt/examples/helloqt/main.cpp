#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label("Hello QT!");
    label.setAlignment(Qt::AlignCenter);
    label.show();

    return app.exec();
}
