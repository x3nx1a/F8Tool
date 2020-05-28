#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>
#include "api.h"
#include <QMainWindow>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
namespace Ui {
class ServerData;
}

class ServerData : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ServerData(QWidget *parent = 0);
    ~ServerData();

    void setServerName(QString serverName) { this->m_sServerName = serverName; }
    void setPackageName(QString packageName) { this->m_sPackageName = packageName; }
    void setExpireDate(QString expireDate) { this->m_sExpireDate = expireDate; }
    void setIsOnline(bool status);
    void init();
    void setServerOnline(service_type type, bool online);
    void setServiceStatus(QString type, bool status);
	void DatabaseInitialize();
    int timerId;
    
private:
    Ui::ServerData *ui;
    QString m_sServerName;
    QString m_sPackageName;
    QString m_sExpireDate;
    bool m_bStatus;
    QWidget* parent;
    api* g_api;
    void setOnline(QWidget * widget);
    void setOffline(QWidget * widget);
    virtual void closeEvent (QCloseEvent *event);


public:
    void setApi(api* _api) { g_api = _api; }

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void quitx();
    void start();
    void restart();
    void stop();
};

#endif // SERVERDATA_H
