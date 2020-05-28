#include "serverdata.h"
#include "ui_serverdata.h"
#include "punishment.h"
#include "database.h"

ServerData::ServerData(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ServerData)
{
	ui->setupUi(this);

	Punishment *punishment = new Punishment(this);

	this->setFixedWidth(this->width());
	this->setFixedHeight(this->height());
	connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui->btnRestart, SIGNAL(clicked()), this, SLOT(restart()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui->cbQueryType, SIGNAL(currentIndexChanged(int)), punishment, SLOT(changed(int)));
	connect(ui->btnQuery, SIGNAL(clicked()), punishment, SLOT(submit()));

	punishment->setText(ui->queryText);
	punishment->setTableWidget(ui->m_pTableWidget);

	connect(ui->m_pTableWidget, SIGNAL(customContextMenuRequested(QPoint)), punishment, SLOT(ContextMenu(QPoint)));
	ui->m_pTableWidget->setMouseTracking(true);
	ui->m_pTableWidget->viewport()->setAttribute(Qt::WA_Hover, true);
}

void ServerData::init()
{
	ui->lblExpires->setText("<font color='black'>Expires on:</font> " + m_sExpireDate);
	ui->lblPackageName->setText(m_sPackageName);

	g_api->checkStatus();

	timerId = startTimer(3000);

	ui->tabWidget->setDisabled(true);

	QStringList queryTypes;
	queryTypes.push_back("CharName");
	queryTypes.push_back("CharID");
	queryTypes.push_back("GuildID");
	queryTypes.push_back("GuildName");
	queryTypes.push_back("UserID");
	queryTypes.push_back("UserName");
	queryTypes.push_back("HWID");
	queryTypes.push_back("IP");

	ui->cbQueryType->addItems(queryTypes);

	QStringList m_TableHeader;

	m_TableHeader << "UserJID" << "UserName" << "CharID" << "CharName" << "GuildID" << "GuildName" << "Hardware ID" << "IP" << "Last Login" << "HWID Ban" << "IP Ban";

	ui->m_pTableWidget->setColumnCount(m_TableHeader.count());
	ui->m_pTableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui->m_pTableWidget->setRowCount(50);
	ui->m_pTableWidget->setHorizontalHeaderLabels(m_TableHeader);
	ui->m_pTableWidget->setStyleSheet("QTableView {selection-background-color: rgb(15,151,202);}");

	ui->m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->m_pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->m_pTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void ServerData::setOnline(QWidget * widget)
{
	widget->setStyleSheet("image: url(:/resources/images/on.png);");
}

void ServerData::setOffline(QWidget * widget)
{
	widget->setStyleSheet("image: url(:/resources/images/off.png);");
}

void ServerData::setIsOnline(bool status)
{
	if (status)
	{
		ui->btnStart->setEnabled(false);
		ui->btnRestart->setEnabled(true);
		ui->btnStop->setEnabled(true);
	}
	else
	{
		ui->btnStart->setEnabled(true);
		ui->btnRestart->setEnabled(false);
		ui->btnStop->setEnabled(false);
	}
}

void ServerData::setServiceStatus(QString type, bool status)
{
	QWidget* data = nullptr;
	if (type == "gateway")
	{
		data = this->ui->gtwIcon;
	}
	else if (type == "download")
	{
		data = this->ui->dwnIcon;
	}
	else if (type == "agent")
	{
		data = this->ui->agntIcon;
	}

	if (status)
		setOnline(data);
	else
		setOffline(data);
}

void ServerData::DatabaseInitialize()
{
	bool connected = Database->connect(
		g_api->m_DatabaseData["sql_shard_ip"].toString(),
		g_api->m_DatabaseData["sql_shard_username"].toString(),
		g_api->m_DatabaseData["sql_shard_password"].toString(),
		g_api->m_DatabaseData["sql_shard_database_name"].toString()
		);

	Database->setAccountDatabase(g_api->m_DatabaseData["sql_account_database_name"].toString());
	Database->setShardDatabase(g_api->m_DatabaseData["sql_shard_database_name"].toString());
	Database->setFilterDatabase("F8Filter");

	ui->tabWidget->setDisabled(!connected);
}

void ServerData::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "F8Tool",
		tr("Are you sure you want to exit the F8Tool?"),
		QMessageBox::No | QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	}
	else {
		killTimer(timerId);
		return QApplication::quit();
	}
}

void ServerData::timerEvent(QTimerEvent *event)
{
	g_api->checkStatus();
}

void ServerData::quitx()
{
	killTimer(timerId);
	return QApplication::quit();
}

void ServerData::setServerOnline(service_type type, bool online)
{
	ui->btnStart->setDisabled(!online);
	ui->btnRestart->setDisabled(online);
	ui->btnStop->setDisabled(online);
}


void ServerData::start()
{
	ui->btnStart->setEnabled(false);
	ui->btnRestart->setEnabled(false);
	ui->btnStop->setEnabled(false);

	g_api->start();
}

void ServerData::stop()
{
	ui->btnStart->setEnabled(false);
	ui->btnRestart->setEnabled(false);
	ui->btnStop->setEnabled(false);
	g_api->stop();
}

void ServerData::restart()
{
	ui->btnStart->setEnabled(false);
	ui->btnRestart->setEnabled(false);
	ui->btnStop->setEnabled(false);
	g_api->restart();
}

ServerData::~ServerData()
{
	killTimer(timerId);
	delete ui;
	QApplication::quit();
}
