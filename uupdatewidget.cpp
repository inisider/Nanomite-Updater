#include "uupdatewidget.h"
#include "ui_uupdatewidget.h"

#include <QProcess>

#include <QStackedWidget>
#include <QMessageBox>
#include <QProgressBar>

#include "ucheckupdateswidget.h"
#include "uupdatesmodel.h"
#include "uupdatestableview.h"
#include "ufiledownloader.h"

#include <QDebug>

UUpdateWidget::UUpdateWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UUpdateWidget)
{
    ui->setupUi(this);

    setFixedSize(size());

    ui->menubar->setHidden(true);
    ui->statusbar->setHidden(true);
    ui->toolBar->addAction(QPixmap(), "Install updates", this, SLOT(slot_downloadFileFinished()));
    ui->toolBar->addAction(QPixmap(), "Quit", this, SLOT(slot_closeWidget()));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QPixmap(), "Check updates", this, SLOT(slot_checkUpdates()));

    m_toolBarActions = ui->toolBar->actions();
    m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(false);

    m_stackedWidget = new QStackedWidget(this);
    m_checkUpdatesWidget = new UCheckUpdatesWidget(m_stackedWidget);
    m_updatesTableView = new UUpdatesTableView(m_stackedWidget);

    m_stackedWidget->addWidget(new QWidget(m_stackedWidget));
    m_stackedWidget->addWidget(m_checkUpdatesWidget);
    m_stackedWidget->addWidget(m_updatesTableView);

    m_stackedWidget->setCurrentIndex(0);

    setCentralWidget(m_stackedWidget);

    connect(m_checkUpdatesWidget,   SIGNAL(signal_processUpdatesFinished(UUpdatesModel *)),
            this,                   SLOT(slot_showUpdatesTable(UUpdatesModel *)));
    connect(m_checkUpdatesWidget,   SIGNAL(signal_downloadFailed(QString)),
            this,                   SLOT(slot_checkUpdatesFailed(QString)));

    m_downloader = new UFileDownloader;

    connect(m_downloader,   SIGNAL(signal_downloadFileFinished()),
            this,           SLOT(slot_downloadFileFinished()));
    connect(m_downloader,   SIGNAL(signal_error(QString)),
            this,           SLOT(slot_error(QString)));

    m_currentDownloadFile = -1;

    connect(this,   SIGNAL(signal_downloadUpdatesFinished()),
            this,   SLOT(slot_installUpdates()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdateWidget::~UUpdateWidget()
{
    delete ui;

    if (m_downloader != 0) {
        delete m_downloader;
        m_downloader = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_installUpdates()
{
    qDebug() << __FUNCTION__;
    m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(true);

    m_stackedWidget->setCurrentIndex(0);
    m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(false);
    m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(true);

    QMessageBox::information(this, tr("Nanomite updater"),
                                    tr("The update is completed"));

    QProcess process;
    process.start("qtNanomite.exe");
    exit(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_checkUpdates()
{
    m_currentDownloadFile = -1;

    m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(false);

    m_stackedWidget->setCurrentIndex(1);
    m_checkUpdatesWidget->checkUpdates();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_checkUpdatesFailed(const QString &error)
{
    QMessageBox msgBox;
    msgBox.setText(error);
    msgBox.exec();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_closeWidget()
{
    exit(0);
//    close();
}

void UUpdateWidget::slot_showUpdatesTable(UUpdatesModel *model)
{
    if (model->rowCount() == 0) {
        qDebug() << "there is no updates";

        QMessageBox::information(this, tr("Nanomite updater"),
                                        tr("There is no updates"));

        m_stackedWidget->setCurrentIndex(0);
        m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(false);
        m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(true);
        return;
    }

    m_updatesTableView->setModel(model);
    m_progressBarList.clear();

    QProgressBar *progressBar;
    for (int i = 0; i < model->rowCount(); i++) {
        progressBar = new QProgressBar(m_updatesTableView);

        progressBar->setTextVisible(false);
        progressBar->setMinimum(0);
        progressBar->setMaximum(0);        

        m_progressBarList.append(progressBar);

        m_updatesTableView->setIndexWidget(model->index(i, UUpdatesModel::eSTATUS), m_progressBarList.last());
    }

    m_updatesTableView->resizeColumnsToContents();

    m_stackedWidget->setCurrentIndex(2);
    m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(true);
    m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_downloadFileFinished()
{
    m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(false);

    m_currentDownloadFile++;

    QAbstractTableModel *model = (UUpdatesModel *)m_updatesTableView->model();

    if (m_currentDownloadFile == model->rowCount()) {
        Q_EMIT signal_downloadUpdatesFinished();

        return;
    }

    m_downloader->set_progressBar(m_progressBarList.at(m_currentDownloadFile));

    QModelIndex index = model->index(m_currentDownloadFile, UUpdatesModel::ePACKAGE);

    QString packageName = model->data(index, Qt::DisplayRole).toString();
    if (packageName.contains('/') == true) {
        int indx = packageName.indexOf('/');
        m_downloader->setDir(packageName.remove(indx, packageName.size() - indx));
    } else {
        m_downloader->setDir("");
    }

    index = model->index(m_currentDownloadFile, UUpdatesModel::eURI);
    m_downloader->slot_downloadFile(model->data(index, Qt::DisplayRole).toUrl());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget::slot_error(const QString &error)
{
    qDebug() << __FUNCTION__ << ':' << error;

    if (error == "Error URL" || error == "Error while downloading file") {
        slot_downloadFileFinished();
    }
}

