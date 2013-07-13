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
#ifndef UFILEDOWNLOADER_H
#define UFILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>
#include <QSslError>

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
    void slot_downloadFile(QUrl url, const QString &neededFileName = "");

private slots:
    void slot_getFileSize(QNetworkReply *pReply);
    void slot_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void slot_downloadFileFinished();
    void slot_readFile();
    void slot_sslErrors(QNetworkReply *reply, QList<QSslError> errorList);

private:
    QNetworkAccessManager   m_manager;
    QUrl                    m_url;
    QFile                   m_file;
    QString                 m_pathDir;
    QNetworkReply           *m_reply;
    QProgressBar            *m_pb;
};

#endif // UFILEDOWNLOADER_H
