#include "mainwindow.h"
#include <QApplication>

#include "udownloader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();

    UDownloader downloader;
    downloader.downloadUpdates();
    
    return a.exec();
}
