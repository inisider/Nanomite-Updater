#include "ucheckupdateswidget.h"
#include "ui_ucheckupdateswidget.h"

#include <QDebug>
#include <QDir>
#include <QCryptographicHash>

#include <QFile>

#include "udownloadmanager.h"
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

    m_downloader = new UDownloadManager;

    // init connections for communicating with class of downloading updates
    connect(m_downloader,   SIGNAL(signal_finished(const QString &)),
            this,           SLOT(slot_downloadFinished(const QString &)));

    m_downloader->append(QUrl(UPDATER_INI_REPO));
    m_downloader->slot_startNextDownload();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_processUpdates()
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

    file.close();

    // add getting size of packages
    // ...

    Q_EMIT signal_processUpdatesFinished(m_updatesModel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_downloadFinished(const QString &msg)
{
    if (msg.isEmpty() == true) {
        slot_processUpdates();

        Q_EMIT signal_downloadFinished();
    } else {
        Q_EMIT signal_downloadFailed(msg);
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
