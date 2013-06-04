#include "ucheckupdateswidget.h"
#include "ui_ucheckupdateswidget.h"

#include "udownloader.h"

#define UPDATER_INI_REPO "https://github.com/zer0fl4g/Nanomite/blob/master/Build/x64/Release/QtCore4.dll?raw=true"

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
