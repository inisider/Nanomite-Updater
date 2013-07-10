#include "ucheckupdateswidget.h"
#include "ui_ucheckupdateswidget.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QCryptographicHash>
#include <QFile>
#include <QProcess>

#include <Windows.h>

#include "ufiledownloader.h"
#include "usettingsreader.h"
#include "uupdatesmodel.h"

#include "..\clsMemManager.h"

#define UPDATER_INI_X64 "https://raw.github.com/zer0fl4g/Nanomite/master/Build/x64/Release/updater.ini"
#define UPDATER_INI_X32 "https://raw.github.com/zer0fl4g/Nanomite/master/Build/Win32/Release/updater.ini"

typedef BOOL (WINAPI *IW64PFP)(HANDLE, BOOL *);

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

    ui->status->setText("Checking for updating...");

    // init connections for communicating with class of downloading updates
    disconnect(m_downloader,    SIGNAL(signal_downloadFileFinished()),
               this,            SLOT(slot_downloadFinished()));
    disconnect(m_downloader,    SIGNAL(signal_gotFileSize(uint)),
               this,            SLOT(slot_getFileSize(uint)));

    connect(m_downloader,   SIGNAL(signal_downloadFileFinished()),
            this,           SLOT(slot_downloadFinished()));

    QUrl url;

#ifdef _AMD64_
    BOOL is64Bit = FALSE;
    IW64PFP IW64P = (IW64PFP)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "IsWow64Process");
    if(IW64P != NULL)
    {
        IW64P(GetCurrentProcess(), &is64Bit);
    }

    if (is64Bit != TRUE) {
        url.setUrl(QString(UPDATER_INI_X64));
    } else {
        url.setUrl(QString(UPDATER_INI_X32));
    }
#else
	url.setUrl(QString(UPDATER_INI_X32));
#endif

    m_downloader->slot_downloadFile(url);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::slot_processUpdates()
{
    ui->status->setText("Process updater.ini file");

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
    settingsReader.readSettings("updater.ini");

    TReadSettings settings = settingsReader.getSettings();
    TReadSettings::const_iterator it = settings.begin();

    // create folder for updates
    QDir currDir(QDir::currentPath());
    QString savePath = currDir.currentPath();
    QString createdFolder = currDir.currentPath() + "/updates";
    currDir.mkdir(createdFolder);
    currDir.setCurrent(createdFolder);

    // check if is it new updates
    while (it != settings.end()) {
        hash.reset();
        file.close();

        file.setFileName(savePath + '/' + it.key());

        if (file.open(QIODevice::ReadOnly) == true) {
            hash.addData(file.readAll());

            if (hash.result().toHex() != it.value().hash) {
                addUpdateToModel(&it.value(), &currentRow);

                qDebug() << "need to be update file: " << file.fileName(); // add file for updating
            }
        } else {
            addUpdateToModel(&it.value(), &currentRow);

            createFolders(it.key());

            qDebug() << "file does not exist: " << file.fileName(); // add file for updating...
        }

        it++;
    }

    file.close();

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

    QModelIndex uriIndex = m_updatesModel->index(m_currentUpdate, UUpdatesModel::eURI);
    QModelIndex packageNameIndex = m_updatesModel->index(m_currentUpdate, UUpdatesModel::ePACKAGE);

    qDebug() << m_updatesModel->data(packageNameIndex, Qt::DisplayRole).toString();
    ui->status->setText(QString("Getting size of file: %1").arg( m_updatesModel->data(packageNameIndex, Qt::DisplayRole).toString()));

    m_downloader->slot_getFileSize(QUrl(m_updatesModel->data(uriIndex, Qt::DisplayRole).toString()));

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
    } else if (msg == "Error URL") {
        slot_gettingFilesSize();
    } else if (msg == "Error while downloading file") {
        QMessageBox::information(this, tr("Nanomite updater"),
                                       tr("Error while downloading of updater.ini"));
        exit(1);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCheckUpdatesWidget::createFolders(const QString &path)
{
    QString tmp = path;
    QString folderName;
    int index;
    QDir currDir(QDir::currentPath());

    QString savePath = currDir.currentPath();
    QString createdFolder;

    // creating all subfolders
    while(tmp.contains('/') == true) {
        index = tmp.indexOf('/');
        folderName = tmp.mid(0, index);
        tmp.remove(0, index + 1);

        createdFolder = currDir.currentPath() + '/' + folderName;

        currDir.mkdir(createdFolder);
        currDir.setCurrent(createdFolder);
    }

    // set root directory
    currDir.setCurrent(savePath);
}
