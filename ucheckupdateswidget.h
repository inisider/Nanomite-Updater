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
    bool updateUpdater(const SSettingsInfo *info);
    bool checkQtNanomiteHash(const SSettingsInfo *info);
    void downloadNewUpdater(const QUrl &url);
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

    void slot_setupNewUpdater();

private:
    Ui::UCheckUpdatesWidget     *ui;
    UFileDownloader             *m_downloader;
    UUpdatesModel               *m_updatesModel;
    unsigned int                m_currentUpdate;
};

#endif // UCHECKUPDATESWIDGET_H
