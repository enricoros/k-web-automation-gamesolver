#include "GameSolver.h"
#include "ScriptEnvironment.h"
#include "ScreenCapture.h"
#include "ui_GameSolverGUI.h"
#include <QFile>

GameSolver::GameSolver(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameSolver)
    , m_script(0)
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

    // capture
    m_capture = new ScreenCapture(ui->webView, this);
    m_capture->setFrequency(10);
    m_capture->setEnabled(true);

    // script env
    m_script = new ScriptEnvironment(m_capture, this);

    // #### TEMP
    ui->webView->load(QUrl("http://apps.new.facebook.com/bejeweledblitz/"));
}

GameSolver::~GameSolver()
{
    delete m_script;
    delete m_capture;
    delete ui;
}

void GameSolver::on_pushButton_2_clicked()
{
    QFile scriptFile("bejeweled.js");
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        qWarning("Can't load bejeweled.js");
        return;
    }
    m_script->setScript(scriptFile.readAll());
}

#include <QLabel>
void GameSolver::on_pushButton_clicked()
{
    QPixmap p = m_capture->capture();
    p = p.scaled(p.width() / 2, p.height() / 2);
    QLabel * lab = new QLabel(0);
    lab->setFixedSize(p.size());
    lab->setPixmap(p);
    lab->show();
}

