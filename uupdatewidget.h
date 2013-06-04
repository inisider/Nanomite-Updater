#ifndef UUPDATEWIDGET_H
#define UUPDATEWIDGET_H

#include <QWidget>
#include <QUrl>

#include "udownloader.h"

namespace Ui {
class UUpdateWidget;
}

class UUpdateWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UUpdateWidget(QWidget *parent = 0);
    ~UUpdateWidget();

private:
    void init();
    void initConnections();

public slots:
    void slot_closeWidget();
    void slot_checkUpdatesAction();

//signals:
//    void signal_installUpdates(const QString &fileName, const QString &filePath);

//public slots:
//    void slot_installUpdates(const QString &fileName, const QString &filePath);

//    void slot_downloadFailed(const QString &error);
//    void slot_downloadFinished(const QString &fileName, const QString &filePath);
//    void slot_redirectTo(const QUrl &redirectedUrl);
//    void slot_unableSaveFile(const QString &fileName, const QString &error);
//    void slot_updateDataReadProgress(const qint64 bytesRead, const qint64 totalBytes);

//#ifndef QT_NO_OPENSSL
//    void slot_sslErrors(const QString &errors);
//#endif

//    void slot_closeWidget();
private:
    enum EToolBarActions {
        eINSTALL_UPDATES_ACTION = 0,
        eQUIT_ACTION,
        eCHECK_UPDATES_ACTION,
        eMAX_ACTION
    };

    Ui::UUpdateWidget   *m_ui;
    UDownloader         m_downloader;

    QList<QAction *>    m_toolBarActions;
};

#endif // UUPDATEWIDGET_H
