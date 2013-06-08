#include "ucheckupdateswidget.h"
#include "ui_ucheckupdateswidget.h"

#include <QDebug>
#include <QDir>
#include <QCryptographicHash>

#include "udownloader.h"
#include "usettingsreader.h"
#include "uupdatesmodel.h"

#define UPDATER_INI_REPO "https://raw.github.com/inisider/Nanomite-Updater/UUpdateWidget/bin/updater.ini"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCheckUpdatesWidget::UCheckUpdatesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UCheckUpdatesWidget), m_downloader(0), m_updatesModel(0)
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

    if (m_updatesModel != 0) {
        delete m_updatesModel;
        m_updatesModel = 0;
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
    if (m_updatesModel != 0) {
        delete m_updatesModel;
        m_updatesModel = 0;
    }

    m_updatesModel = new UUpdatesModel;

    int currentRow = 0;

    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile file;

    USettingsReader settingsReader;
    settingsReader.readSettings("updater.ini.tmp");

    TReadSettings settings = settingsReader.getSettings();

    TReadSettings::const_iterator it = settings.begin();

    // check if is it new updates
    while (it != settings.end()) {
        file.setFileName(QDir::currentPath() + '/' + it.key());

        if (file.exists()) {
            hash.addData(file.readAll());

            if (hash.result() != it.value().hash) {
                addUpdateToModel(&it.value(), &currentRow);

                qDebug() << "need to be update file: " << file.fileName(); // add file for updating
            }
        } else {
            addUpdateToModel(&it.value(), &currentRow);

            qDebug() << "file does not exist: " << file.fileName(); // add file for updating...
        }

        it++;
    }

    qDebug() << m_updatesModel->rowCount();

    // get sizes of files that need for updating
    // ...
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::addUpdateToModel(const SSettingsInfo *info, int *currentRow)
{
    QModelIndex index;

    m_updatesModel->insertRows(*currentRow, 1);

    index = m_updatesModel->index(*currentRow, UUpdatesModel::ePACKAGE);
    m_updatesModel->setData(index, info->fileName, Qt::DisplayRole);

    index = m_updatesModel->index(*currentRow, UUpdatesModel::eURI);
    m_updatesModel->setData(index, info->link, Qt::DisplayRole);

    (*currentRow)++;
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
