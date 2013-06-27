#include "ustartupdater.h"

#include <QDebug>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UStartUpdater::UStartUpdater(const QString &fileName, QObject *parent)
    : QObject(parent), m_fileName(fileName)
{
    init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UStartUpdater::launchUpdater()
{
    m_process.start(m_fileName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UStartUpdater::init()
{
    connect(&m_process, SIGNAL(finished(int)),
            this,       SLOT(slot_checkExitCode(int)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UStartUpdater::slot_checkExitCode(int exitCode)
{
    qDebug() << exitCode;

    // 0 - return if all is OK
    // 1 - if there is some error
    // 2 - if need to update updater.exe ( this case is handle inner of updater.exe )

    // 3 - means that this is update of QtNanomite.exe and for install QtNanomite.exe need to close process of QtNanomite.exe.
    if (exitCode == 3) {
        m_process.start(m_fileName, QStringList() << "startUpdate");
        exit(0);
    }
}
