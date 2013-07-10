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
#ifndef UCHECKUPDATESWIDGET_H
#define UCHECKUPDATESWIDGET_H

#include <QWidget>

#include <QUrl>

namespace Ui {
class UCheckUpdatesWidget;
}

class UUpdatesModel;
struct SSettingsInfo;
class UFileDownloader;

class UCheckUpdatesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UCheckUpdatesWidget(QWidget *parent = 0);
    ~UCheckUpdatesWidget();

    void checkUpdates();

private:
    void addUpdateToModel(const SSettingsInfo *info, int *currentRow);
    void createFolders(const QString &path);

signals:
    void signal_downloadFailed(const QString &);
    void signal_downloadFinished(const QString &msg = "");
    void signal_processUpdatesFinished(UUpdatesModel *model);

protected slots:
    void slot_processUpdates();

    void slot_downloadFinished();
    void slot_gettingFilesSize();
    void slot_getFileSize(unsigned int size);

    void slot_downloaderError(const QString &msg);

private:
    Ui::UCheckUpdatesWidget     *ui;
    UFileDownloader             *m_downloader;
    UUpdatesModel               *m_updatesModel;
    unsigned int                m_currentUpdate;
};

#endif // UCHECKUPDATESWIDGET_H
