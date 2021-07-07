#include "mainwindow.h"
#include <QApplication>


QSize size(800,600);
QFont font(QString("Arial"),16);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
