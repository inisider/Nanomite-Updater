#include <QApplication>

#include <QFile>
#include <QProcess>
#include <QMessageBox>

#include "uupdatewidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMessageBox::information(0, "Nanomite updater",
                             QString("%1").arg(argc));


    if (argc == 2) {
        if (strcpy(argv[1], "update") == 0) {
            QFile::remove("updater.exe");
            QFile::copy("updater_tmp.exe", "updater.exe");
            QProcess process;
            process.start("updater.exe");
            exit(1);
        }
    }

    QFile file;

    file.setFileName("updater_tmp.exe");
    if (file.exists() == true) {
        QFile::remove("updater_tmp.exe");
    }

    UUpdateWidget w;
    w.show();

    return a.exec();
}
