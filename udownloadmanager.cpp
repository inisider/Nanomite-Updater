#include "udownloadmanager.h"

#include <QFileInfo>
#include <QStringList>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QDebug>

UDownloadManager::UDownloadManager(QObject *parent) :
    QObject(parent)
{
}

UDownloadManager::~UDownloadManager()
{
}

void UDownloadManager::append(const QUrl &url)
{
    m_downloadQueue.enqueue(url);
}

void UDownloadManager::append(const QStringList &urlList)
{
    foreach (QString url, urlList)
        append(QUrl::fromEncoded(url.toLocal8Bit()));
}

QString UDownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        QFile::remove(basename);
//        // already exists, don't overwrite
//        int i = 0;
//        basename += '.';
//        while (QFile::exists(basename + QString::number(i)))
//            ++i;

//        basename += QString::number(i);
    }

    return basename;
}

void UDownloadManager::slot_startNextDownload()
{
    m_file.close();
    if (m_downloadQueue.isEmpty()) {
        Q_EMIT signal_finished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();

    QString fileName = saveFileName(url);
    m_file.setFileName(fileName);
    if (!m_file.open(QIODevice::WriteOnly)) {
        qDebug() << "error while opening file";

        Q_EMIT signal_finished(QString("Problem opening save file %1 for download %2: %3")
                               .arg(fileName).arg(url.toEncoded().constData()).arg(m_file.errorString()));

        slot_startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);
    m_currentDownload = m_manager.get(request);

    connect(m_currentDownload,  SIGNAL(downloadProgress(qint64,qint64)),
                                SLOT(slot_downloadProgress(qint64,qint64)));
    connect(m_currentDownload,  SIGNAL(finished()),
                                SLOT(slot_downloadFinished()));
    connect(m_currentDownload,  SIGNAL(readyRead()),
                                SLOT(slot_downloadReadyRead()));
}

void UDownloadManager::slot_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
//    Q_EMIT signal_downloadProgress()
//    progressBar.setStatus(bytesReceived, bytesTotal);

//    // calculate the download speed
//    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
//    QString unit;
//    if (speed < 1024) {
//        unit = "bytes/sec";
//    } else if (speed < 1024*1024) {
//        speed /= 1024;
//        unit = "kB/s";
//    } else {
//        speed /= 1024*1024;
//        unit = "MB/s";
//    }

//    progressBar.setMessage(QString::fromLatin1("%1 %2")
//                           .arg(speed, 3, 'f', 1).arg(unit));
//    progressBar.update();
}

void UDownloadManager::slot_downloadFinished()
{
    QVariant redirectionTarget = m_currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_currentDownload->error()) {
        qDebug() << "error while downloading";
        Q_EMIT signal_finished(QString("Failed: %1").arg(m_currentDownload->errorString()));
    } else if (!redirectionTarget.isNull()) {
        qDebug() << "redirected link: " << redirectionTarget.toUrl();
        m_downloadQueue.enqueue(redirectionTarget.toUrl());
    } else {
        qDebug() << "finished success";
        Q_EMIT signal_finished(); // finished success
    }

    m_currentDownload->deleteLater();
//    slot_startNextDownload();
}

void UDownloadManager::slot_downloadReadyRead()
{
    qDebug() << "readyRead";
    m_file.write(m_currentDownload->readAll());
}
