#include "udownloader.h"

#include "settings.h"

#include <QtNetwork>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UDownloader::UDownloader(QObject *parent) :
    QObject(parent), m_url(QString(repository))
{    
    connect(&m_qnam,    SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this,       SLOT(slot_authenticationRequired(QNetworkReply*,QAuthenticator*)));
#ifndef QT_NO_OPENSSL
    connect(&m_qnam,    SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this,       SLOT(slot_sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::downloadUpdates()
{
    QFileInfo fileInfo(m_url.path());
    QString fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "name";

    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    m_file = new QFile(fileName);
    if (!m_file->open(QIODevice::WriteOnly)) {
        Q_EMIT signal_unableSaveFile(fileName, m_file->errorString());

        delete m_file;
        m_file = 0;
        return;
    }

    // schedule the request
    m_httpRequestAborted = false;
    startRequest();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::setUrl(QUrl url)
{
    m_url = url;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::startRequest()
{
    m_reply = m_qnam.get(QNetworkRequest(m_url));

    connect(m_reply,    SIGNAL(finished()),
            this,       SLOT(slot_httpFinished()));
    connect(m_reply,    SIGNAL(readyRead()),
            this,       SLOT(slot_httpReadyRead()));
    connect(m_reply,    SIGNAL(downloadProgress(qint64,qint64)),
            this,       SLOT(slot_updateDataReadProgress(qint64,qint64)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::slot_httpFinished()
{
    if (m_httpRequestAborted) {
        if (m_file) {
            m_file->close();
            m_file->remove();
            delete m_file;
            m_file = 0;
        }
        m_reply->deleteLater();

        Q_EMIT signal_downloadFinished();

        return;
    }

    m_file->flush();
    m_file->close();

    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_reply->error()) {
        m_file->remove();

        Q_EMIT signal_downloadFailed(m_reply->errorString());
    } else if (!redirectionTarget.isNull()) {
        Q_EMIT signal_redirectTo(m_url.resolved(redirectionTarget.toUrl()));
    } else {
        QString fileName = QFileInfo(QUrl(QString(repository)).path()).fileName();
        Q_EMIT signal_downloadFinished(fileName, QDir::currentPath());
    }

    m_reply->deleteLater();
    m_reply = 0;
    delete m_file;
    m_file = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::slot_httpReadyRead()
{
    if (m_file)
        m_file->write(m_reply->readAll());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::slot_updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (m_httpRequestAborted)
        return;

    Q_EMIT signal_updateDataReadProgress(bytesRead, totalBytes);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::slot_authenticationRequired(QNetworkReply *, QAuthenticator *)
{
    // FIXME: think about realization of authentication
    // now there is no sense for this possobility
//    QDialog dlg;
//    Ui::Dialog ui;
//    ui.setupUi(&dlg);
//    dlg.adjustSize();
//    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(url.host()));

//    // Did the URL have information? Fill the UI
//    // This is only relevant if the URL-supplied credentials were wrong
//    ui.userEdit->setText(url.userName());
//    ui.passwordEdit->setText(url.password());

//    if (dlg.exec() == QDialog::Accepted) {
//        authenticator->setUser(ui.userEdit->text());
//        authenticator->setPassword(ui.passwordEdit->text());
//    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef QT_NO_OPENSSL
void UDownloader::slot_sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    Q_EMIT signal_sslErrors(errorString);
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::slot_redirectTo(QUrl newUrl)
{
    m_url = newUrl;
    m_reply->deleteLater();
    m_file->open(QIODevice::WriteOnly);
    m_file->resize(0);
    startRequest();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UDownloader::slot_ignoreSslErrors()
{
    m_reply->ignoreSslErrors();
}
