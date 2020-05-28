#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>
#include <QSettings>
#include "api.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setLoginBtnEnable();
    
private:
    Ui::MainWindow *ui;
    api* g_api;
    virtual void closeEvent (QCloseEvent *event);
    QString m_sSettingsFile;

    void loadSettings();

    void saveSettings();

private slots:
    void login();
};

#endif // MAINWINDOW_H
