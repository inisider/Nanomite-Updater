#ifndef UFILEDOWNLOADER_H
#define UFILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>

class QProgressBar;

class UFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit UFileDownloader(QObject *parent = 0);
    virtual ~UFileDownloader();
    
    void set_progressBar(QProgressBar *pb);
    void setDir(const QString &pathDir);

private:
    QString saveFileName(const QUrl &url);

signals:
    void signal_error(const QString &msg);
    void signal_gotFileSize(unsigned int fileSize);
    void signal_downloadFileFinished();

    // just for internal usage
    void signal_redirectTo(QUrl url);
    void signal_fileSizeredirectTo(QUrl url);

public slots:
    void slot_getFileSize(QUrl url);
    void slot_downloadFile(QUrl url);

private slots:
    void slot_getFileSize(QNetworkReply *pReply);
    void slot_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void slot_downloadFileFinished();
    void slot_readFile();

private:
    QNetworkAccessManager   m_manager;
    QUrl                    m_url;
    QFile                   m_file;
    QString                 m_pathDir;
    QNetworkReply           *m_reply;
    QProgressBar            *m_pb;
};

#endif // UFILEDOWNLOADER_H
