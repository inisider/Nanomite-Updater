#include "mainwindow.h"
#include <QApplication>

#include "uupdatewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();
    UUpdateWidget w;
    w.show();

    return a.exec();
}
