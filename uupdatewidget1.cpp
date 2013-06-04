#include "uupdatewidget1.h"
#include "ui_uupdatewidget1.h"

#include <QStackedWidget>
#include <QMessageBox>

#include "ucheckupdateswidget.h"

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

    m_stackedWidget->addWidget(new QWidget(m_stackedWidget));
    m_stackedWidget->addWidget(m_checkUpdatesWidget);

    m_stackedWidget->setCurrentIndex(0);

    setCentralWidget(m_stackedWidget);

    connect(m_checkUpdatesWidget,   SIGNAL(signal_downloadFinished(QString,QString)),
            this,                   SLOT(slot_installUpdates()));
    connect(m_checkUpdatesWidget,   SIGNAL(signal_downloadFailed(QString)),
            this,                   SLOT(slot_checkUpdatesFailed(QString)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdateWidget1::~UUpdateWidget1()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUpdateWidget1::slot_installUpdates()
{
    m_stackedWidget->setCurrentIndex(0);
    m_toolBarActions.at(eINSTALL_UPDATES_ACTION)->setEnabled(true);
    m_toolBarActions.at(eCHECK_UPDATES_ACTION)->setEnabled(false);
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

