#include <QApplication>

#include "uupdatewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UUpdateWidget w;
    w.show();

//    w.slot_installUpdates("master", "/home/inisider/Projects/Nanomite-Updater/bin");

    return a.exec();
}
