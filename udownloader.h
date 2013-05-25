#ifndef UDOWNLOADER_H
#define UDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QFile;
class QSslError;
class QAuthenticator;
class QNetworkReply;
QT_END_NAMESPACE

class UDownloader : public QObject
{
    Q_OBJECT
public:
    explicit UDownloader(QObject *parent = 0);

    void downloadUpdates();

private:
    void startRequest();
    
private:
    QUrl                    m_url;
    QNetworkAccessManager   m_qnam;
    QNetworkReply           *m_reply;
    QFile                   *m_file;
    int                     m_httpGetId;
    bool                    m_httpRequestAborted;

signals:
    void signal_unableSaveFile(QString fileName, QString error);
    void signal_downloadFailed(QString error);
    void signal_downloadFinished(QString fileName = "", QString filePath = "");
    void signal_updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void signal_sslErrors(QString errorString);
    void signal_redirectTo(QUrl newUrl);
    
public slots:
    void slot_ignoreSslErrors();

    void slot_redirectTo(QUrl newUrl);

private slots:
    void slot_httpFinished();
    void slot_httpReadyRead();
    void slot_updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void slot_authenticationRequired(QNetworkReply*, QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void slot_sslErrors(QNetworkReply*,const QList<QSslError> &errors);
#endif
    
};

#endif // UDOWNLOADER_H
