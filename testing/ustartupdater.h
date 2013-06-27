#ifndef USTARTUPDATER_H
#define USTARTUPDATER_H

#include <QObject>
#include <QProcess>

class UStartUpdater : public QObject
{
    Q_OBJECT
public:
    UStartUpdater(const QString &fileName, QObject *parent = 0);

    void launchUpdater();

private:
    void init();

private Q_SLOTS:
    void slot_checkExitCode(int exitCode);

private:
    QProcess m_process;
    QString m_fileName;
};

#endif // USTARTUPDATER_H
