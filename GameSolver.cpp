#include "GameSolver.h"
#include "ScreenCapture.h"
#include "ui_GameSolverGUI.h"

GameSolver::GameSolver(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameSolver)
    , m_scriptEngine(0)
{
    // create ui
    ui->setupUi(this);
    connect(ui->webView, SIGNAL(loadStarted()), ui->progressBar, SLOT(show()));
    connect(ui->webView, SIGNAL(loadFinished(bool)), ui->progressBar, SLOT(hide()));
    connect(ui->webView, SIGNAL(loadProgress(int)), ui->progressBar, SLOT(setValue(int)));
    ui->progressBar->hide();

    // setup browser
    QWebSettings * settings = ui->webView->settings();
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, true);

    // #### TEMP
    ui->webView->load(QUrl("http://www.facebook.com"));
}

GameSolver::~GameSolver()
{
    delete m_scriptEngine;
    delete ui;
}

#include <QLabel>
void GameSolver::on_pushButton_clicked()
{
    QPixmap p = ScreenCapture(ui->webView).capture();
    p = p.scaled(p.width() / 2, p.height() / 2);
    QLabel * lab = new QLabel(0);
    lab->setFixedSize(p.size());
    lab->setPixmap(p);
    lab->show();
}

