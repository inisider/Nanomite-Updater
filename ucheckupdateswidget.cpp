#include "ucheckupdateswidget.h"
#include "ui_ucheckupdateswidget.h"

#include <QDebug>
#include <QDir>
#include <QCryptographicHash>

#include "udownloader.h"
#include "usettingsreader.h"

#define UPDATER_INI_REPO "https://raw.github.com/inisider/Nanomite-Updater/UUpdateWidget/bin/updater.ini"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCheckUpdatesWidget::UCheckUpdatesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UCheckUpdatesWidget), m_downloader(0)
{
    ui->setupUi(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCheckUpdatesWidget::~UCheckUpdatesWidget()
{
    delete ui;

    if (m_downloader != 0) {
        delete m_downloader;
        m_downloader = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::checkUpdates()
{
    if (m_downloader != 0) {
        delete m_downloader;
        m_downloader = 0;
    }

    m_downloader = new UDownloader;

    // init connections for communicating with class of downloading updates
    connect(m_downloader,  SIGNAL(signal_downloadFailed(const QString &)),
            this,          SLOT(slot_downloadFailed(const QString &)));
    connect(m_downloader,  SIGNAL(signal_downloadFinished(const QString &,const QString &)),
            this,          SLOT(slot_downloadFinished(const QString &, const QString &)));
    connect(m_downloader,  SIGNAL(signal_redirectTo(const QUrl &)),
            this,          SLOT(slot_redirectTo(const QUrl &)));
    connect(m_downloader,  SIGNAL(signal_unableSaveFile(const QString &,const QString &)),
            this,          SLOT(slot_unableSaveFile(const QString &, const QString &)));

    // start of working downloading updates
    m_downloader->setUrl(QUrl(QString(UPDATER_INI_REPO)));
    m_downloader->downloadUpdates();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::processUpdates()
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile file;

    USettingsReader settingsReader;
    settingsReader.readSettings("updater.ini.tmp");

    TReadSettings settings = settingsReader.getSettings();

    TReadSettings::const_iterator it = settings.begin();

    while (it != settings.end()) {
        file.setFileName(QDir::currentPath() + '/' + it.key());

        if (file.exists()) {
            hash.addData(file.readAll());

            if (hash.result() == it.value().hash) {
                qDebug() << "hash is sample";
            } else {
                qDebug() << "need to be update file: " << file.fileName();
            }
        } else {
            qDebug() << "file does not exist: " << file.fileName();
        }

        it++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_downloadFailed(const QString &error)
{
    Q_EMIT signal_downloadFailed(error);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_downloadFinished(const QString &fileName, const QString &filePath)
{
    if (!fileName.isEmpty() && !filePath.isEmpty()) {
        processUpdates();

        Q_EMIT signal_downloadFinished(fileName, filePath);
    } else {
        Q_EMIT signal_downloadFailed("error while downloading");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_redirectTo(const QUrl &redirectedUrl)
{
    m_downloader->slot_redirectTo(redirectedUrl);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_unableSaveFile(const QString &fileName, const QString &error)
{
    Q_EMIT signal_downloadFailed(QString("Something wrong while saving file %1; error: %2").arg(fileName).arg(error));
}
