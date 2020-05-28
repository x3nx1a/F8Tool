#include "api.h"
#include <QDesktopServices>
#include "mainwindow.h"
#include "serverdata.h"

api::api()
{
    m_pVersion = new QNetworkAccessManager();
    QObject::connect(m_pVersion, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(versionRequestFinished(QNetworkReply*)));

    m_pServerStart = new QNetworkAccessManager(this);
    QObject::connect(m_pServerStart, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(startRequestFinished(QNetworkReply*)));

    m_pServerRestart = new QNetworkAccessManager(this);
    QObject::connect(m_pServerRestart, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(restartRequestFinished(QNetworkReply*)));

    m_pServerStop = new QNetworkAccessManager(this);
    QObject::connect(m_pServerStop, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(stopRequestFinished(QNetworkReply*)));

    m_pLogin = new QNetworkAccessManager(this);
    QObject::connect(m_pLogin, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(loginRequestFinished(QNetworkReply*)));

    m_pDevelopmentMode = new QNetworkAccessManager(this);
    QObject::connect(m_pDevelopmentMode, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(developmentModeRequestFinished(QNetworkReply*)));

    m_pServerStatus = new QNetworkAccessManager(this);
    QObject::connect(m_pServerStatus, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(serverStatusRequestFinished(QNetworkReply*)));

    m_pServerData = new QNetworkAccessManager(this);
    QObject::connect(m_pServerData, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(serverDataRequestFinished(QNetworkReply*)));
}

void api::versionCheck()
{
    QString version_api_url(def_api_url "version?");
    version_api_url.append("version=" STR(MAJOR_VER) "." STR(MINOR_VER));
    m_pVersion->get(QNetworkRequest(version_api_url));
}

void api::login(QWidget* widget, QString api_key, QString api_hash)
{
    QString login_api_url(def_api_url "login?");
    login_api_url.append("api_key="+api_key);
    login_api_url.append("&api_hash="+api_hash);
    login_api_url.append("&version=" STR(MAJOR_VER) "." STR(MINOR_VER));
    QUrl url(login_api_url);
    m_pLogin->get(QNetworkRequest(login_api_url));

    m_pWidget = widget;
}

void api::start()
{
    QString request_url(def_api_url "start?");
    request_url.append("api_key="+this->api_key);
    request_url.append("&api_hash="+this->api_hash);
    request_url.append("&version=" STR(MAJOR_VER) "." STR(MINOR_VER));
    QUrl url(request_url);
    qDebug() << request_url;
    m_pServerStart->get(QNetworkRequest(url));
}
void api::stop()
{
    QString request_url(def_api_url "stop?");
    request_url.append("api_key="+this->api_key);
    request_url.append("&api_hash="+this->api_hash);
    request_url.append("&version=" STR(MAJOR_VER) "." STR(MINOR_VER));
    QUrl url(request_url);
    m_pServerStop->get(QNetworkRequest(url));
    qDebug() << request_url;
}
void api::restart()
{
    QString request_url(def_api_url "restart?");
    request_url.append("api_key="+this->api_key);
    request_url.append("&api_hash="+this->api_hash);
    request_url.append("&version=" STR(MAJOR_VER) "." STR(MINOR_VER));
    QUrl url(request_url);
    m_pServerRestart->get(QNetworkRequest(url));
    qDebug() << request_url;
}

void api::checkStatus()
{
    QString request_url(def_api_url "status?");
    request_url.append("api_key="+this->api_key);
    request_url.append("&api_hash="+this->api_hash);
    request_url.append("&version=" STR(MAJOR_VER) "." STR(MINOR_VER));
    QUrl url(request_url);
    m_pServerStatus->get(QNetworkRequest(url));
    qDebug() << request_url;
}

void api::versionRequestFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError) {

           QString strReply = (QString)reply->readAll();

           QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

           QJsonObject jsonObject = jsonResponse.object();

           bool versionStatus = jsonObject["status"].toBool();
           QString versionMessage = jsonObject["message"].toString();
           bool requiredUpdate = jsonObject["required_update"].toBool();
           QString updateUrl = jsonObject["update_link"].toString();

           if(versionStatus)
           {

           }
           else
           {
               showMessageBox(e_tool,versionMessage,true);
               if(requiredUpdate)
                   QDesktopServices::openUrl(QUrl(updateUrl, QUrl::TolerantMode));
           }
       } else {
        showMessageBox(e_api,"Could not connect the f8api. Please try again.",true);
       }

       delete reply;
}

void api::loginRequestFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError) {

           QString strReply = (QString)reply->readAll();

           QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

           QJsonObject jsonObject = jsonResponse.object();

           bool status = jsonObject["status"].toBool();
           QString error = jsonObject["message"].toString();
           bool exit = jsonObject["exit"].toBool();

           if(status)
           {
               m_pWidget->setVisible(false);

               data = reinterpret_cast<ServerData*>(m_pSWidget);

			   reinterpret_cast<ServerData*>(data)->setServerName(jsonObject["server_name"].toString());
               reinterpret_cast<ServerData*>(data)->setPackageName(jsonObject["package"].toString());
               reinterpret_cast<ServerData*>(data)->setExpireDate(jsonObject["expire_date"].toString());
               reinterpret_cast<ServerData*>(data)->setApi(this);
               reinterpret_cast<ServerData*>(data)->setIsOnline(jsonObject["server_status"].toBool());
               reinterpret_cast<ServerData*>(data)->init();
               reinterpret_cast<ServerData*>(data)->setWindowFlags( (reinterpret_cast<ServerData*>(data)->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

               this->api_key = jsonObject["key"].toString();
               this->api_hash = jsonObject["hash"].toString();
			   reinterpret_cast<ServerData*>(data)->show();
               //data->raise();
               //data->activateWindow();


               QString data_api_url(def_api_url "data?");
               data_api_url.append("api_key="+api_key);
               data_api_url.append("&api_hash="+api_hash);
               data_api_url.append("&version=" STR(MAJOR_VER) "." STR(MINOR_VER));
               QUrl url(data_api_url);
               m_pServerData->get(QNetworkRequest(data_api_url));

               reinterpret_cast<MainWindow*>(m_pWidget)->setLoginBtnEnable();
           }
           else
           {
               showMessageBox(e_tool,error,exit);
           }
       } else {
        showMessageBox(e_api,"Could not connect the f8api. Please try again.",true);
       }

       delete reply;
}

void api::serverDataRequestFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError) {

           QString strReply = (QString)reply->readAll();

           QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

           this->m_DatabaseData = jsonResponse.object();
		   reinterpret_cast<ServerData*>(data)->DatabaseInitialize();

       } else {
        showMessageBox(e_api,"Could not connect the f8api. Please try again.",true);
       }

       delete reply;
}

void api::serverStatusRequestFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError) {

           QString strReply = (QString)reply->readAll();

           QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

           QJsonObject jsonObject = jsonResponse.object();

           bool status = jsonObject["status"].toBool();
           QString error = jsonObject["message"].toString();
           bool exit = jsonObject["exit"].toBool();
           bool srv_status = jsonObject["server_status"].toBool();


           bool gateway = jsonObject["gateway"].toBool();
           bool download = jsonObject["download"].toBool();
           bool agent = jsonObject["agent"].toBool();

           ServerData* widget = reinterpret_cast<ServerData*>(m_pSWidget);
           widget->setServiceStatus("gateway", gateway);
           widget->setServiceStatus("download", download);
           widget->setServiceStatus("agent", agent);

       } else {
        showMessageBox(e_api,"Could not connect the f8api. Please try again.",true);
       }

       delete reply;
}

void api::serverAction(QNetworkReply* reply, service_type type)
{
    if(reply->error() == QNetworkReply::NoError) {

           QString strReply = (QString)reply->readAll();

           QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

           QJsonObject jsonObject = jsonResponse.object();

           bool status = jsonObject["status"].toBool();
           QString error = jsonObject["message"].toString();
           bool exit = jsonObject["exit"].toBool();
           bool srv_status = jsonObject["server_status"].toBool();

           ServerData* widget = reinterpret_cast<ServerData*>(m_pSWidget);
           widget->setIsOnline(srv_status);

           showMessageBox(e_api,error,exit);

       } else {
        showMessageBox(e_api,"Could not connect the f8api. Please try again.",true);
       }

       delete reply;
}

void api::startRequestFinished(QNetworkReply* reply)
{
    this->serverAction(reply,s_start);
}

void api::restartRequestFinished(QNetworkReply* reply)
{
    this->serverAction(reply,s_restart);
}
void api::stopRequestFinished(QNetworkReply* reply)
{
    this->serverAction(reply,s_stop);
}
void api::showMessageBox(error type, QString message, bool _exit)
{
    QMessageBox msgBox;
    switch((error)type)
    {
        default:
        case e_tool:
            msgBox.setWindowTitle("F8Tool");
            break;
        case e_api:
            msgBox.setWindowTitle("F8Api");
            break;
        case e_version:
            msgBox.setWindowTitle("F8Tool Version");
            break;

    }
    msgBox.setText(message);
    msgBox.exec();
    if(_exit)
        QCoreApplication::quit();
}
