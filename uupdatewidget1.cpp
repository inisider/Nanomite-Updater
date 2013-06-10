#include "uupdatewidget1.h"
#include "ui_uupdatewidget1.h"

#include <QStackedWidget>
#include <QMessageBox>
#include <QProgressBar>

#include "ucheckupdateswidget.h"
#include "uupdatesmodel.h"
#include "uupdatestableview.h"
#include "udownloader.h"

#include <QDebug>

UUpdateWidget1::UUpdateWidget1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UUpdateWidget1)
{
    ui->setupUi(this);

    setFixedSize(size());

    ui->menubar->setHidden(true);
    ui->statusbar->setHidden(true);
    ui->toolBar->addAction(QPixmap(), "Install updates", this, SLOT(slot_installUpdates()));
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

    m_downloader = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdateWidget1::~UUpdateWidget1()
{
    delete ui;

    if (m_downloader != 0) {
        delete m_downloader;
        m_downloader = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget1::slot_installUpdates()
{
    if (m_downloader != 0) {
        delete m_downloader;
        m_downloader = 0;
    }

//    m_downloader = new UDownloader;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget1::slot_checkUpdates()
{
    m_stackedWidget->setCurrentIndex(1);
    m_checkUpdatesWidget->checkUpdates();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget1::slot_checkUpdatesFailed(const QString &error)
{
    QMessageBox msgBox;
    msgBox.setText(error);
    msgBox.exec();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget1::slot_closeWidget()
{
    close();
}

void UUpdateWidget1::slot_showUpdatesTable(UUpdatesModel *model)
{
    if (model->rowCount() == 0) {
        qDebug() << "there is no updates";
        m_stackedWidget->setCurrentIndex(0);
        m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(false);
        m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(true);
        return;
    }

    m_updatesTableView->setModel(model);

    QProgressBar *progressBar;
    for (int i = 0; i < model->rowCount(); i++) {
        progressBar = new QProgressBar(m_updatesTableView);

        progressBar->setTextVisible(false);
        progressBar->setMaximum(0);
        progressBar->setMinimum(0);

        m_progressBarList.append(progressBar);

        m_updatesTableView->setIndexWidget(model->index(i, UUpdatesModel::eSTATUS), m_progressBarList.last());
    }

    m_updatesTableView->resizeColumnsToContents();

    m_stackedWidget->setCurrentIndex(2);
    m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(true);
    m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(false);
}

