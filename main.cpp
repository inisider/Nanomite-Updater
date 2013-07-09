#include <QApplication>

#include <QFile>

#include <QProcess>

#include "uupdatewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UUpdateWidget w;

    w.show();

    return a.exec();
}
