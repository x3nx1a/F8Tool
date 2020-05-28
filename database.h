#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

class CDatabase
{
public:
	CDatabase();
	~CDatabase();

	bool connect(const QString& Host, const QString& Username, const QString& Password, const QString& DefaultDatabase);
	static CDatabase* getInstance() { return &databaseInstance; }
private:
	QSqlDatabase db;
public:
	QSqlDatabase getDatabase() { return this->db; }
	QSqlDatabase account();
	QSqlDatabase shard();
	QSqlDatabase filter();

	void setAccountDatabase(const QString& name) { this->accountDatabase = name; }
	void setShardDatabase(const QString& name) { this->shardDatabase = name; }
	void setFilterDatabase(const QString& name) { this->filterDatabase = name; }
private:
	static CDatabase databaseInstance;
	QString accountDatabase;
	QString shardDatabase;
	QString filterDatabase;
};

#define Database CDatabase::getInstance()
#endif // DATABASE_H
