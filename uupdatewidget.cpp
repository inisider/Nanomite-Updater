#include "uupdatewidget.h"
#include "ui_uupdatewidget.h"

#include "udownloader.h"

#include <QDir>
#include <QDebug>

#include <QMessageBox>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdateWidget::UUpdateWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::UUpdateWidget), m_downloader(new UDownloader(this))
{
    m_ui->setupUi(this);

    setWindowFlags(Qt::WindowTitleHint);
    setFixedSize(size());

    init();
    initConnections();

    m_downloader->downloadUpdates();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdateWidget::~UUpdateWidget()
{
    delete m_ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_downloadFailed(const QString &error)
{
    QMessageBox::information(this, tr("HTTP"),
                             tr("Download failed: %1.")
                             .arg(error));
    slot_closeWidget();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_downloadFinished(const QString &fileName, const QString &filePath)
{
    if (!fileName.isEmpty() && !filePath.isEmpty()) {
        Q_EMIT signal_installUpdates(fileName, filePath);
    } else {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed"));
        slot_closeWidget();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_redirectTo(const QUrl &redirectedUrl)
{
    slot_updateDataReadProgress(0, 0);

    if (QMessageBox::question(this, tr("HTTP"),
                              tr("Redirect to %1 ?").arg(redirectedUrl.toString()),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_downloader->slot_redirectTo(redirectedUrl);
    } else {
        slot_closeWidget();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_unableSaveFile(const QString &fileName, const QString &error)
{
    QMessageBox::information(this, tr("HTTP"),
                             tr("Unable to save the file %1: %2.")
                             .arg(fileName).arg(error));
    slot_closeWidget();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_updateDataReadProgress(const qint64 bytesRead, const qint64 totalBytes)
{
    m_ui->m_progressBar->setMaximum(totalBytes);
    m_ui->m_progressBar->setValue(bytesRead);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_sslErrors(const QString &errors)
{
    if (QMessageBox::warning(this, tr("HTTP"),
                             tr("One or more SSL errors has occurred: %1").arg(errors),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        m_downloader->slot_ignoreSslErrors();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_closeWidget()
{
    close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::init()
{
    m_ui->m_currActionL->setText("Downloading updates");
    m_ui->m_progressBar->setValue(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::initConnections()
{
    // init connections for communicating with class of downloading updates
    connect(m_downloader,   SIGNAL(signal_downloadFailed(const QString &)),
            this,           SLOT(slot_downloadFailed(const QString &)));
    connect(m_downloader,   SIGNAL(signal_downloadFinished(const QString &,const QString &)),
            this,           SLOT(slot_downloadFinished(const QString &, const QString &)));
    connect(m_downloader,   SIGNAL(signal_redirectTo(const QUrl &)),
            this,           SLOT(slot_redirectTo(const QUrl &)));
    connect(m_downloader,   SIGNAL(signal_unableSaveFile(const QString &,const QString &)),
            this,           SLOT(slot_unableSaveFile(const QString &, const QString &)));
    connect(m_downloader,   SIGNAL(signal_updateDataReadProgress(const qint64, const qint64)),
            this,           SLOT(slot_updateDataReadProgress(const qint64, const qint64)));
#ifndef QT_NO_OPENSSL
    connect(m_downloader,   SIGNAL(signal_sslErrors(const QString &)),
            this,           SLOT(slot_sslErrors(const QString &)));
#endif

    // init connection for installing updates
    connect(this,           SIGNAL(signal_installUpdates(const QString &,const QString &)),
            this,           SLOT(slot_installUpdates(const QString &,const QString &)));

    // init connection for ...
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_installUpdates(const QString &fileName, const QString &filePath)
{
    Q_UNUSED(fileName);
    Q_UNUSED(filePath);
////    QString destinationDir = filePath;
////    QDir baseDir(destinationDir);

//    QZipReader unzip(fileName, QIODevice::ReadOnly);
//    unzip.extractAll(filePath);
//    unzip.close();
////    QList<QZipReader::FileInfo> allFiles = unzip.fileInfoList();
////    QZipReader::FileInfo fi;

////    foreach (QZipReader::FileInfo fi, allFiles)
////    {
////        const QString absPath = destinationDir + QDir::separator() + fi.filePath;
////        if (fi.isDir)
////        {
////            if (!baseDir.mkpath(fi.filePath))
////                 return;
////            if (!QFile::setPermissions(absPath, fi.permissions))
////                return;
////        }
////    }

////    foreach (QZipReader::FileInfo fi, allFiles)
////    {
////        const QString absPath = destinationDir + QDir::separator() + fi.filePath;
////        if (fi.isFile)
////        {
////            QFile file(absPath);
////            if( file.open(QFile::WriteOnly) )
////            {
////                QApplication::setOverrideCursor(Qt::WaitCursor);
////                file.write(unzip.fileData(fi.filePath), unzip.fileData(fi.filePath).size());
////                file.setPermissions(fi.permissions);
////                QApplication::restoreOverrideCursor();
////                file.close();
////            }
////        }
////    }
////    unzip.close();

    slot_closeWidget();
}
