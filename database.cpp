#include "database.h"

CDatabase CDatabase::databaseInstance;

CDatabase::CDatabase()
{
	this->db = QSqlDatabase::addDatabase("QODBC");
}

CDatabase::~CDatabase()
{

}

bool CDatabase::connect(const QString& Host, const QString& Username, const QString& Password, const QString& DefaultDatabase)
{
	this->db = QSqlDatabase::addDatabase("QODBC");
	QString connectString = "Driver={SQL Server Native Client 11.0};";
	connectString.append("Server="+Host+";");
	connectString.append("Database=" + DefaultDatabase + ";");
	connectString.append("UID=" + Username + ";");
	connectString.append("PWD=" + Password + ";");
	//qDebug() << connectString;
	db.setDatabaseName(connectString);
	if (db.open())
	{
		return true;
	}
	else {
		//if(!db.isDriverAvailable("QODBC"))
		//	qDebug() << "DRIVER NOT AVAILABLE";
		//qDebug() << db.lastError().driverText();
		//qDebug() << db.lastError().databaseText();
		//QString s(QSqlDatabase::drivers().join("."));
		//qDebug() << s;
		return false;
	}
}

QSqlDatabase CDatabase::account()
{
	this->db.setDatabaseName(accountDatabase);
	return this->db;
}

QSqlDatabase CDatabase::shard()
{
	this->db.setDatabaseName(shardDatabase);
	return this->db;
}

QSqlDatabase CDatabase::filter()
{
	this->db.setDatabaseName(filterDatabase);
	return this->db;
}

