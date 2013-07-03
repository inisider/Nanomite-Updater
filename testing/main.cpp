#include <QCoreApplication>
#include "ustartupdater.h"

#define UPDATER_PATH "../../bin/updater.exe"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    UStartUpdater launchUpdater(UPDATER_PATH);
    launchUpdater.launchUpdater();

    return a.exec();
}
