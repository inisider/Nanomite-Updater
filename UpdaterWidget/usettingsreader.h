/*
 * 	This file is part of Nanomite.
 *
 *    Nanomite is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Nanomite is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Nanomite.  If not, see <http://www.gnu.org/licenses/>.
 */
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
