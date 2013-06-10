#ifndef UCHECKUPDATESWIDGET_H
#define UCHECKUPDATESWIDGET_H

#include <QWidget>

#include <QUrl>

namespace Ui {
class UCheckUpdatesWidget;
}

class UUpdatesModel;
struct SSettingsInfo;
class UDownloadManager;

class UCheckUpdatesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UCheckUpdatesWidget(QWidget *parent = 0);
    ~UCheckUpdatesWidget();

    void checkUpdates();

private:
    void addUpdateToModel(const SSettingsInfo *info, int *currentRow);

signals:
    void signal_downloadFailed(const QString &);
    void signal_downloadFinished(const QString &msg = "");
    void signal_processUpdatesFinished(UUpdatesModel *model);

protected slots:
    void slot_processUpdates();

    void slot_downloadFinished(const QString &msg);

private:
    Ui::UCheckUpdatesWidget     *ui;
    UDownloadManager          *m_downloader;
    UUpdatesModel               *m_updatesModel;
};

#endif // UCHECKUPDATESWIDGET_H
