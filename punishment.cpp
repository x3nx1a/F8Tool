#include "punishment.h"
#include "database.h"
#include <QObject>
#include <qaction.h>
#include <qmenu.h>
#include <qclipboard.h>
#include <qapplication.h>
#include <unordered_map>


Punishment::Punishment(QObject *parent)
	: QObject(parent)
{
}

Punishment::~Punishment()
{
}

void Punishment::changed(int index)
{
	this->currentType = (query_type)index;
}

void Punishment::submit()
{
	QString strQuery = "select usr.UserJID, accjid.AccountID as UserName ,chr.CharID, chr.CharName16 as CharName, chr.GuildID, gld.Name as GuildName, lel.HardwareID,lel.IPAddress,max(lel.EventTime) as EventTime, count(hb.HardwareID) as HBStatus, count(ipb.IPAddress) as IPBStatus \
		from _Char(nolock) chr \
		inner join _User(nolock) usr on usr.CharID = chr.CharID \
		inner join _AccountJID(nolock) accjid on accjid.JID = usr.UserJID \
		inner join _Guild(nolock) gld on gld.ID = chr.GuildID \
		inner join(select UserJID, HardwareID, IPAddress, max(EventTime) as EventTime from F8Filter.._LogEventLogin(nolock) group by UserJID, HardwareID, IPAddress) lel on lel.UserJID = accjid.JID \
		left join F8Filter.._HardwareBan(nolock) hb on hb.HardwareID = lel.HardwareID \
		left join F8Filter.._IPBan(nolock) ipb on ipb.IPAddress = lel.IPAddress \
		where %1 = '%2' \
		group by usr.UserJID, accjid.AccountID, chr.CharID, chr.CharName16, chr.GuildID, gld.Name, lel.HardwareID, lel.IPAddress, lel.EventTime \
		order by EventTime desc";
	switch (this->currentType)
	{
	case query_type::q_charname:
		strQuery = strQuery.arg("chr.CharName16");
		break;
	case query_type::q_charid:
		strQuery = strQuery.arg("chr.CharID");
		break;
	case query_type::q_guildid:
		strQuery = strQuery.arg("chr.GuildID");
		break;
	case query_type::q_guildname:
		strQuery = strQuery.arg("gld.Name");
		break;
	case query_type::q_hwid:
		strQuery = strQuery.arg("lel.HardwareID");
		break;
	case query_type::q_ip:
		strQuery = strQuery.arg("lel.HardwareID");
		break;
	case query_type::q_userid:
		strQuery = strQuery.arg("usr.UserJID");
		break;
	case query_type::q_username:
		strQuery = strQuery.arg("accjid.AccountID");
		break;
	}
	strQuery = strQuery.arg(this->queryText->text());

	QSqlQuery query = Database->shard().exec(strQuery);

	int UserJID = query.record().indexOf("UserJID");
	int UserName = query.record().indexOf("UserName");
	int CharID = query.record().indexOf("CharID");
	int CharName = query.record().indexOf("CharName");
	int GuildID = query.record().indexOf("GuildID");
	int GuildName = query.record().indexOf("GuildName");
	int HardwareID = query.record().indexOf("HardwareID");
	int IPAddress = query.record().indexOf("IPAddress");
	int EventTime = query.record().indexOf("EventTime");
	int HBStatus = query.record().indexOf("HBStatus");
	int IPBStatus = query.record().indexOf("IPBStatus");

	this->tableWidget->setRowCount(query.numRowsAffected());

	this->tableWidget->clearContents();
	int i = 0;

	while (query.next()) {
		this->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(UserJID).toString()));
		this->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(UserName).toString()));
		this->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(CharID).toString()));
		this->tableWidget->setItem(i, 3, new QTableWidgetItem(query.value(CharName).toString()));
		this->tableWidget->setItem(i, 4, new QTableWidgetItem(query.value(GuildID).toString()));
		this->tableWidget->setItem(i, 5, new QTableWidgetItem(query.value(GuildName).toString()));
		this->tableWidget->setItem(i, 6, new QTableWidgetItem(query.value(HardwareID).toString()));
		this->tableWidget->setItem(i, 7, new QTableWidgetItem(query.value(IPAddress).toString()));
		this->tableWidget->setItem(i, 8, new QTableWidgetItem(query.value(EventTime).toString()));
		this->tableWidget->setItem(i, 9, new QTableWidgetItem(query.value(HBStatus).toBool() ? "1" : "0"));
		this->tableWidget->setItem(i, 10, new QTableWidgetItem(query.value(IPBStatus).toBool() ? "1" : "0"));
		if (query.value(HBStatus).toBool())
			tableWidget->item(i, 9)->setBackground(Qt::red);
		else
			tableWidget->item(i, 9)->setBackground(Qt::green);
		if (query.value(IPBStatus).toBool())
			tableWidget->item(i, 10)->setBackground(Qt::red);
		else
			tableWidget->item(i, 10)->setBackground(Qt::green);
		i++;
	}
}
void Punishment::ContextMenu(QPoint position)
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();

	if (selectedItemList.size() == 0)
		return;
	//this->tableWidget->item(selectedItemList.at, 9);

	/*foreach(QModelIndex itemIndex, selectedItemList)
	{
	}*/

	Q_UNUSED(position);
	QAction *UserJID = new QAction("Copy User JID", this);
	connect(UserJID, SIGNAL(triggered()), this, SLOT(copyUserJID()));

	QAction *UserName = new QAction("Copy User Name", this);
	connect(UserName, SIGNAL(triggered()), this, SLOT(copyUserName()));

	QAction *CharID = new QAction("Copy Char ID", this);
	connect(CharID, SIGNAL(triggered()), this, SLOT(copyCharID()));

	QAction *CharName = new QAction("Copy Char Name", this);
	connect(CharName, SIGNAL(triggered()), this, SLOT(copyCharName()));

	QAction *GuildID = new QAction("Copy Guild ID", this);
	connect(GuildID, SIGNAL(triggered()), this, SLOT(copyGuildID()));

	QAction *GuildName = new QAction("Copy Guild Name", this);
	connect(GuildName, SIGNAL(triggered()), this, SLOT(copyGuildName()));

	QAction *HardwareID = new QAction("Copy Hardware ID", this);
	connect(HardwareID, SIGNAL(triggered()), this, SLOT(copyHWID()));

	QAction *IPAddress = new QAction("Copy IP Address", this);
	connect(IPAddress, SIGNAL(triggered()), this, SLOT(copyIP()));

	QAction *LastLogin = new QAction("Copy Last Login", this);
	connect(LastLogin, SIGNAL(triggered()), this, SLOT(copyLastLogin()));

	QAction *BanHWID = new QAction("Ban HWID", this);
	connect(BanHWID, SIGNAL(triggered()), this, SLOT(BanHWID()));

	QAction *UnBanHWID = new QAction("Un-Ban HWID", this);
	connect(UnBanHWID, SIGNAL(triggered()), this, SLOT(UnBanHWID()));

	QAction *BanIP = new QAction("Ban IP", this);
	connect(BanIP, SIGNAL(triggered()), this, SLOT(BanIP()));

	QAction *UnBanIP = new QAction("Un-Ban IP", this);
	connect(UnBanIP, SIGNAL(triggered()), this, SLOT(UnBanIP()));

	QAction *AllBanHWID = new QAction("Ban HWID", this);
	connect(AllBanHWID, SIGNAL(triggered()), this, SLOT(AllBanHWID()));

	QAction *AllUnBanHWID = new QAction("Un-Ban HWID", this);
	connect(AllUnBanHWID, SIGNAL(triggered()), this, SLOT(AllUnBanHWID()));

	QAction *AllBanIP = new QAction("Ban IP", this);
	connect(AllBanIP, SIGNAL(triggered()), this, SLOT(AllBanIP()));

	QAction *AllUnBanIP = new QAction("Un-Ban IP", this);
	connect(AllUnBanIP, SIGNAL(triggered()), this, SLOT(AllUnBanIP()));

	QMenu *pContextMenu = new QMenu(this->tableWidget);
	pContextMenu->addSection("Copy Actions");
	pContextMenu->addAction(UserJID);
	pContextMenu->addAction(UserName);
	pContextMenu->addAction(CharID);
	pContextMenu->addAction(CharName);
	pContextMenu->addAction(GuildID);
	pContextMenu->addAction(GuildName);
	pContextMenu->addAction(HardwareID);
	pContextMenu->addAction(IPAddress);
	pContextMenu->addAction(LastLogin);

	pContextMenu->addSeparator();

	pContextMenu->addSection("Selected Row");
	if (selectedItemList[9].data().toBool())
		pContextMenu->addAction(UnBanHWID);
	else
		pContextMenu->addAction(BanHWID);

	if (selectedItemList[10].data().toBool())
		pContextMenu->addAction(UnBanIP);
	else
		pContextMenu->addAction(BanIP);

	pContextMenu->addSeparator();

	pContextMenu->addSection("All Rows");
	pContextMenu->addAction(AllBanHWID);
	pContextMenu->addAction(AllUnBanHWID);
	pContextMenu->addAction(AllBanIP);
	pContextMenu->addAction(AllUnBanIP);

	pContextMenu->exec(QCursor::pos());
}

void Punishment::copyUserJID()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[0].data().toString());
}

void Punishment::copyUserName()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[1].data().toString());
}

void Punishment::copyCharID()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[2].data().toString());
}

void Punishment::copyCharName()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[3].data().toString());
}

void Punishment::copyGuildID()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[4].data().toString());
}

void Punishment::copyGuildName()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[5].data().toString());
}

void Punishment::copyHWID()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[6].data().toString());
}

void Punishment::copyIP()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[7].data().toString());
}

void Punishment::copyLastLogin()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();
	this->setClipboard(selectedItemList[8].data().toString());
}

void Punishment::BanHWID()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();

	QString strQuery = "insert into F8Filter.._HardwareBan values('%1')";
	strQuery = strQuery.arg(selectedItemList[6].data().toString());
	QTableWidgetItem* hwid = tableWidget->item(this->tableWidget->selectionModel()->selectedRows()[0].row(), 9);

	QSqlQuery query = Database->filter().exec(strQuery);
	this->updateAllHWIDRows(selectedItemList[6].data().toString(), true);
}

void Punishment::UnBanHWID()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();

	QString strQuery = "delete from F8Filter.._HardwareBan where HardwareID = '%1'";
	strQuery = strQuery.arg(selectedItemList[6].data().toString());
	QTableWidgetItem* hwid = tableWidget->item(this->tableWidget->selectionModel()->selectedRows()[0].row(), 9);

	QSqlQuery query = Database->filter().exec(strQuery);
	this->updateAllHWIDRows(selectedItemList[6].data().toString(), false);
}

void Punishment::BanIP()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();

	QString strQuery = "insert into F8Filter.._IPBan values('%1')";
	strQuery = strQuery.arg(selectedItemList[7].data().toString());
	QTableWidgetItem* hwid = tableWidget->item(this->tableWidget->selectionModel()->selectedRows()[0].row(), 10);

	QSqlQuery query = Database->filter().exec(strQuery);
	this->updateAllIPRows(selectedItemList[7].data().toString(), true);
}

void Punishment::UnBanIP()
{
	QModelIndexList selectedItemList = this->tableWidget->selectionModel()->selectedIndexes();

	QString strQuery = "delete from F8Filter.._IPBan where IPAddress = '%1'";
	strQuery = strQuery.arg(selectedItemList[7].data().toString());
	QTableWidgetItem* hwid = tableWidget->item(this->tableWidget->selectionModel()->selectedRows()[0].row(), 10);

	QSqlQuery query = Database->filter().exec(strQuery);
	this->updateAllIPRows(selectedItemList[7].data().toString(), false);
}

void Punishment::AllBanHWID()
{
	std::unordered_map<std::string, bool> hwids;
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* hwid = tableWidget->item(row, 6);
		hwids[hwid->data(0).toString().toStdString()] = false;
	}

	for (auto &hwid : hwids)
	{
		QString strQuery = "insert into F8Filter.._HardwareBan values('%1')";
		strQuery = strQuery.arg(hwid.first.c_str());
		QSqlQuery query = Database->filter().exec(strQuery);
		this->updateAllHWIDRows(hwid.first.c_str(), true);
	}
}

void Punishment::AllUnBanHWID()
{
	std::unordered_map<std::string, bool> hwids;
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* hwid = tableWidget->item(row, 6);
		hwids[hwid->data(0).toString().toStdString()] = false;
	}

	for (auto &hwid : hwids)
	{
		QString strQuery = "delete from F8Filter.._HardwareBan where HardwareID = '%1'";
		strQuery = strQuery.arg(hwid.first.c_str());
		QSqlQuery query = Database->filter().exec(strQuery);
		this->updateAllHWIDRows(hwid.first.c_str(), false);
	}
}

void Punishment::AllBanIP()
{
	std::unordered_map<std::string, bool> ips;
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* ip = tableWidget->item(row, 7);
		ips[ip->data(0).toString().toStdString()] = false;
	}

	for (auto &ip : ips)
	{
		QString strQuery = "insert into F8Filter.._IPBan values('%1')";
		strQuery = strQuery.arg(ip.first.c_str());
		QSqlQuery query = Database->filter().exec(strQuery);
		this->updateAllIPRows(ip.first.c_str(), true);
	}
}

void Punishment::AllUnBanIP()
{
	std::unordered_map<std::string, bool> ips;
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* ip = tableWidget->item(row, 7);
		ips[ip->data(0).toString().toStdString()] = false;
	}

	for (auto &ip : ips)
	{
		QString strQuery = "delete from F8Filter.._IPBan where IPAddress = '%1'";
		strQuery = strQuery.arg(ip.first.c_str());
		QSqlQuery query = Database->filter().exec(strQuery);
		this->updateAllIPRows(ip.first.c_str(), false);
	}
}

void Punishment::setClipboard(const QString & text)
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(text);
}

void Punishment::updateAllRows()
{
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* hwid = tableWidget->item(row, 6);
		QTableWidgetItem* ip = tableWidget->item(row, 7);

		QTableWidgetItem* hwidstatus = tableWidget->item(row, 9);
		QTableWidgetItem* ipstatus = tableWidget->item(row, 10);

		if (hwidstatus->data(0).toBool())
			hwidstatus->setBackground(Qt::red);
		else
			hwidstatus->setBackground(Qt::green);

		if (ipstatus->data(0).toBool())
			ipstatus->setBackground(Qt::red);
		else
			ipstatus->setBackground(Qt::green);
	}
}

void Punishment::updateAllHWIDRows(QString _hwid, bool newValue)
{
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* hwid = tableWidget->item(row, 6);
		if (hwid->data(0).toString() == _hwid)
		{
			QTableWidgetItem* hwidstatus = tableWidget->item(row, 9);

			if (newValue)
			{
				hwidstatus->setData(0, QString("1"));
				hwidstatus->setBackground(Qt::red);
			}
			else
			{
				hwidstatus->setData(0, QString("0"));
				hwidstatus->setBackground(Qt::green);
			}

		}
	}
}

void Punishment::updateAllIPRows(QString _ip, bool newValue)
{
	for (int row = 0; row < tableWidget->rowCount(); ++row) {
		QTableWidgetItem* ip = tableWidget->item(row, 7);

		if (ip->data(0).toString() == _ip)
		{
			QTableWidgetItem* ipstatus = tableWidget->item(row, 10);

			if (newValue)
			{
				ipstatus->setData(0, QString("1"));
				ipstatus->setBackground(Qt::red);
			}
			else
			{
				ipstatus->setData(0, QString("0"));
				ipstatus->setBackground(Qt::green);
			}
		}
	}
}
