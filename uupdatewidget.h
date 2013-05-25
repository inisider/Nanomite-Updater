#ifndef UUPDATEWIDGET_H
#define UUPDATEWIDGET_H

#include <QWidget>
#include <QUrl>

namespace Ui {
class UUpdateWidget;
}

class UDownloader;

class UUpdateWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UUpdateWidget(QWidget *parent = 0);
    ~UUpdateWidget();

private:
    void init();
    void initConnections();

signals:
    void signal_installUpdates();

public slots:
    void slot_installUpdates();

    void slot_downloadFailed(const QString &error);
    void slot_downloadFinished(const QString &fileName, const QString &filePath);
    void slot_redirectTo(const QUrl &redirectedUrl);
    void slot_unableSaveFile(const QString &fileName, const QString &error);
    void slot_updateDataReadProgress(const qint64 bytesRead, const qint64 totalBytes);

#ifndef QT_NO_OPENSSL
    void slot_sslErrors(const QString &errors);
#endif

    void slot_closeWidget();
private:
    Ui::UUpdateWidget   *m_ui;
    UDownloader         *m_downloader;
};

#endif // UUPDATEWIDGET_H
