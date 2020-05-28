#ifndef API_H
#define API_H
#include <globals.h>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QStringList>
#include <QString>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>
#include <QObject>
#include <QWidget>
class api : public QObject
{
    Q_OBJECT

public:
        /*static api& getInstance()
        {
            static api instance;
            return instance;
        }
*/
        enum error
        {
            e_tool,
            e_api,
            e_version
        };


        api();
    private:
        QString api_key;
        QString api_hash;
		QObject * data;

        void showMessageBox(error type, QString message, bool _exit = 0);
    private:
        QWidget* m_pWidget;

    public:
        QWidget* m_pSWidget;
        //api(api const&) = delete;
        //void operator=(api const&) = delete;
        QNetworkAccessManager* m_pVersion;
        QNetworkAccessManager* m_pServerStart;
        QNetworkAccessManager* m_pServerStop;
        QNetworkAccessManager* m_pServerRestart;
        QNetworkAccessManager* m_pDevelopmentMode;
        QNetworkAccessManager* m_pLogin;
        QNetworkAccessManager* m_pServerStatus;
        QNetworkAccessManager* m_pServerData;

        void versionCheck();
        void login(QWidget* widget, QString api_key, QString api_hash);
        void start();
        void stop();
        void restart();
        void checkStatus();

        QJsonObject m_DatabaseData;


private slots:
        void versionRequestFinished(QNetworkReply* reply);
        void loginRequestFinished(QNetworkReply* reply);
        void startRequestFinished(QNetworkReply* reply);
        void restartRequestFinished(QNetworkReply* reply);
        void stopRequestFinished(QNetworkReply* reply);
        void serverAction(QNetworkReply* reply, service_type type);
        void serverStatusRequestFinished(QNetworkReply* reply);
        void serverDataRequestFinished(QNetworkReply* reply);
};

#endif // API_H
