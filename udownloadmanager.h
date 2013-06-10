#ifndef UDOWNLOADMANAGER_H
#define UDOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QUrl>
#include <QNetworkAccessManager>

class UDownloadManager: public QObject
{
    Q_OBJECT
public:
    UDownloadManager(QObject *parent = 0);
    ~UDownloadManager();

    void append(const QUrl &url);
    void append(const QStringList &urlList);
    QString saveFileName(const QUrl &url);

signals:
    void signal_finished(QString msg = "");

public slots:
    void slot_startNextDownload();

private slots:    
    void slot_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void slot_downloadFinished();
    void slot_downloadReadyRead();

private:
    QNetworkAccessManager   m_manager;
    QQueue<QUrl>            m_downloadQueue;
    QNetworkReply           *m_currentDownload;
    QFile                   m_file;

//    int                     m_downloadedCount;
//    int                     m_totalCount;
};

#endif // UDOWNLOADMANAGER_H
