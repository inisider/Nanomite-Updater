#ifndef UCHECKUPDATESWIDGET_H
#define UCHECKUPDATESWIDGET_H

#include <QWidget>

#include <QUrl>

namespace Ui {
class UCheckUpdatesWidget;
}

class UDownloader;

class UCheckUpdatesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UCheckUpdatesWidget(QWidget *parent = 0);
    ~UCheckUpdatesWidget();

    void checkUpdates();

private:
    void processUpdates();

signals:
    void signal_downloadFailed(const QString &error);
    void signal_downloadFinished(const QString &fileName, const QString &filePath);

protected slots:
    void slot_downloadFailed(const QString &error);
    void slot_downloadFinished(const QString &fileName, const QString &filePath);
    void slot_redirectTo(const QUrl &redirectedUrl);
    void slot_unableSaveFile(const QString &fileName, const QString &error);

private:
    Ui::UCheckUpdatesWidget     *ui;
    UDownloader                 *m_downloader;
};

#endif // UCHECKUPDATESWIDGET_H
