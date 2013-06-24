#ifndef USETTINGSREADER_H
#define USETTINGSREADER_H

#include <QString>
#include <QMap>

struct SSettingsInfo {
    QString fileName;
    QString link;
    QByteArray hash;

    SSettingsInfo() {
        fileName = "";
        link = "";
        hash = "";
    }
};

typedef QMap<QString, SSettingsInfo> TReadSettings;

class USettingsReader
{
public:
    USettingsReader();

    void readSettings(const QString &fileName);
    TReadSettings getSettings();


private:
    TReadSettings m_readSettings;

};

#endif // USETTINGSREADER_H
