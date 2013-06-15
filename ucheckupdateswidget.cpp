#include "ucheckupdateswidget.h"
#include "ui_ucheckupdateswidget.h"

#include <QDebug>
#include <QDir>
#include <QCryptographicHash>

#include <QFile>

#include "ufiledownloader.h"
#include "usettingsreader.h"
#include "uupdatesmodel.h"

#define UPDATER_INI_REPO "https://raw.github.com/inisider/Nanomite-Updater/UUpdateWidget/bin/updater.ini"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCheckUpdatesWidget::UCheckUpdatesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UCheckUpdatesWidget), m_updatesModel(0), m_currentUpdate(0)
{
    ui->setupUi(this);

    m_downloader = new UFileDownloader;

    connect(m_downloader,   SIGNAL(signal_error(QString)),
            this,           SLOT(slot_downloaderError(QString)));
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
    qDebug() << __FUNCTION__;

//    if (m_downloader != 0) {
//        delete m_downloader;
//        m_downloader = 0;
//    }

//    m_downloader = new UDownloadManager;

    // init connections for communicating with class of downloading updates
    disconnect(m_downloader,    SIGNAL(signal_downloadFileFinished()),
               this,            SLOT(slot_downloadFinished()));
    disconnect(m_downloader,    SIGNAL(signal_gotFileSize(uint)),
               this,            SLOT(slot_getFileSize(uint)));

    connect(m_downloader,   SIGNAL(signal_downloadFileFinished()),
            this,           SLOT(slot_downloadFinished()));

    m_downloader->slot_downloadFile(QUrl(UPDATER_INI_REPO));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_processUpdates()
{
    m_currentUpdate = 0;

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

    file.close();

    // add getting size of packages
    // ...
    slot_gettingFilesSize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_downloadFinished()
{
    slot_processUpdates();
    Q_EMIT signal_downloadFinished();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_gettingFilesSize()
{
    if (m_currentUpdate == (m_updatesModel->rowCount())) {
        Q_EMIT signal_processUpdatesFinished(m_updatesModel);

        return;
    }

    disconnect(m_downloader,    SIGNAL(signal_downloadFileFinished()),
               this,            SLOT(slot_downloadFinished()));
    disconnect(m_downloader,    SIGNAL(signal_gotFileSize(uint)),
               this,            SLOT(slot_getFileSize(uint)));

    connect(m_downloader,       SIGNAL(signal_gotFileSize(uint)),
            this,               SLOT(slot_getFileSize(uint)));


    QModelIndex index = m_updatesModel->index(m_currentUpdate, UUpdatesModel::eURI);

    QModelIndex indx = m_updatesModel->index(m_currentUpdate, UUpdatesModel::ePACKAGE);
    qDebug() << m_updatesModel->data(indx, Qt::DisplayRole).toString();

    m_downloader->slot_getFileSize(QUrl(m_updatesModel->data(index, Qt::DisplayRole).toString()));

    m_currentUpdate++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_getFileSize(unsigned int size)
{
    QModelIndex index = m_updatesModel->index(m_currentUpdate - 1, UUpdatesModel::eSIZE);
    m_updatesModel->setData(index, size, Qt::DisplayRole);

    slot_gettingFilesSize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_downloaderError(const QString &msg)
{
    if (msg == "Error while getting size of the file") {
        slot_gettingFilesSize();
    } else {
        qDebug() << msg;
    }
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
