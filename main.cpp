#include <QApplication>

#include <QFile>
#include <QProcess>

#include "uupdatewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UUpdateWidget w;

    if (argc == 2) {
        if (strcmp(argv[1], "update") == 0) {
            QFile::remove("updater.exe");
            QFile::copy("updater_tmp.exe", "updater.exe");
            QProcess process;
            process.start("updater.exe");
            exit(1);
        }
    }

    bool isFileDeleted = false;
    QFile file;

    file.setFileName("updater_tmp.exe");
    if (file.exists() == true) {
        isFileDeleted = true;
        QFile::remove("updater_tmp.exe");
    }

    if (isFileDeleted == true) {
        w.show();
        w.slot_checkUpdates();
    } else {
        w.show();
    }

    return a.exec();
}
