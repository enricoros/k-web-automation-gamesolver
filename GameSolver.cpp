#include "GameSolver.h"
#include "ScriptEnvironment.h"
#include "ScreenCapture.h"
#include "ui_GameSolverGUI.h"
#include <QDir>
#include <QFile>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

//BEGIN Cookie Permanent Storage
class CookieStore : public QNetworkCookieJar {
    public:
        CookieStore(QObject *parent = 0)
            : QNetworkCookieJar(parent)
            , m_fileName(QDir::toNativeSeparators(QDir::homePath()) + QDir::separator() + ".temp-cookies")
        {
            loadFromDisk();
        }

        ~CookieStore()
        {
            flushToDisk();
        }

        void clear()
        {
            setAllCookies(QList<QNetworkCookie>());
        }

        void loadFromDisk()
        {
            // open the file and open a data stream over it
            QFile cookiesFile(m_fileName);
            if (!cookiesFile.open(QIODevice::ReadOnly)) {
                qWarning("CookieStore::loadFromDisk: error loading cookies from '%s'", qPrintable(m_fileName));
                return;
            }
            QDataStream cs(&cookiesFile);

            // read all cookies
            QList<QNetworkCookie> cookieList;
            while (!cs.atEnd()) {
                QByteArray rawCookie;
                cs >> rawCookie;
                cookieList.append(QNetworkCookie::parseCookies(rawCookie));
            }

            // set the internal cookie list
            setAllCookies(cookieList);
        }

        void flushToDisk() const
        {
            // open the file and open a data stream over it
            QFile cookiesFile(m_fileName);
            if (!cookiesFile.open(QIODevice::WriteOnly)) {
                qWarning("CookieStore::flushToDisk: error saving cookies to '%s'", qPrintable(m_fileName));
                return;
            }
            QDataStream cs(&cookiesFile);

            // write all cookies
            foreach (QNetworkCookie cookie, allCookies())
                cs << cookie.toRawForm();
        }

    private:
        QString m_fileName;
};
//END Cookie Permanent Storage


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
    QWebSettings * webSettings = ui->webView->settings();
    webSettings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    webSettings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    webSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
    webSettings->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebPage * webPage = ui->webView->page();
    webPage->networkAccessManager()->setCookieJar(new CookieStore());

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
    QString fileName = "testscript.js";
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        qWarning("Can't load %s", qPrintable(fileName));
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

