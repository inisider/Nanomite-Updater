#include <QApplication>

#include "uupdatewidget1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UUpdateWidget1 w;
    w.show();

    return a.exec();
}
