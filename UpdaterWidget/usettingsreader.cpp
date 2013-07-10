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
#include "usettingsreader.h"

#include <QSettings>
#include <QStringList>

#include "..\clsMemManager.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
USettingsReader::USettingsReader()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void USettingsReader::readSettings(const QString &fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    SSettingsInfo currSetting;

//    for (int i = 0; i < settings.childGroups().size(); i++) {
//        currSetting.fileName = settings.childGroups().at(i);
//        currSetting.link = settings.value(currSetting.fileName + "/link").toString();
//        currSetting.hash = settings.value(currSetting.fileName + "/md5").toByteArray();

//        m_readSettings[currSetting.fileName] = currSetting;
//    }


    // This is ugly way, because I did not find way how to read NAME section that may look like:
    // [folder/fileName]
    // Way above can read NAME section that may look like: [fileName]
    QString tmp;
    int indx;
    for (int i = 0; i < settings.allKeys().size(); i += 2) {
        tmp = settings.allKeys().at(i);

        indx = tmp.indexOf("/link");

        currSetting.fileName = tmp.remove(indx, tmp.size() - indx);
        currSetting.link = settings.value(currSetting.fileName + "/link").toString();
        currSetting.hash = settings.value(currSetting.fileName + "/md5").toByteArray();

        m_readSettings[currSetting.fileName] = currSetting;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
TReadSettings USettingsReader::getSettings()
{
    return m_readSettings;
}



