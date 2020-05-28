#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverdata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    g_api = new api();
    g_api->versionCheck();
    g_api->m_pSWidget = new ServerData();
    g_api->m_pSWidget->setWindowFlags( Qt::Window);
    g_api->m_pSWidget->setWindowModality( Qt::ApplicationModal );

    //this->setWindowTitle("F8Tool v"STR(MAJOR_VER)"."STR(MINOR_VER));
    this->setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

    ui->setupUi(this);

    this->setFixedWidth(this->width());
    this->setFixedHeight(this->height());

    connect(ui->login_submit, SIGNAL (clicked()),this, SLOT (login()));

    m_sSettingsFile = QApplication::applicationDirPath() + "/settings.ini";
    //qDebug() << m_sSettingsFile;
    loadSettings();
}


void MainWindow::loadSettings()
{
 QSettings settings(m_sSettingsFile, QSettings::IniFormat);
 this->ui->login_api_key->setText(settings.value("API_KEY").toString());
 this->ui->login_api_hash->setText(settings.value("API_HASH").toString());
}

void MainWindow::saveSettings()
{
 QSettings settings(m_sSettingsFile, QSettings::IniFormat);
 settings.setValue("API_KEY", this->ui->login_api_key->text());
 settings.setValue("API_HASH", this->ui->login_api_hash->text());
 settings.sync();
}

void MainWindow::setLoginBtnEnable()
{
    this->ui->login_submit->setEnabled(true);
}


void MainWindow::login()
{
    this->ui->login_submit->setEnabled(false);
    saveSettings();
    g_api->login(this, ui->login_api_key->text(), ui->login_api_hash->text());
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "F8Tool",
                                                                tr("Are you sure you want to exit the F8Tool?"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        return QApplication::quit();
    }
}

MainWindow::~MainWindow()
{
    ServerData* data = reinterpret_cast<ServerData*>(g_api->m_pSWidget);
    g_api->m_pSWidget->killTimer(data->timerId);
    delete ui;
}
