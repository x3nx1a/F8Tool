#pragma once

#include <QObject>
#include <qdebug.h>
#include <qlineedit.h>
#include <qtablewidget.h>

class Punishment : public QObject
{
	Q_OBJECT

public:
	Punishment(QObject *parent);
	~Punishment();
	enum query_type
	{
		q_charname,
		q_charid,
		q_guildid,
		q_guildname,
		q_userid,
		q_username,
		q_hwid,
		q_ip,
	};

	void setText(QLineEdit* edit) { this->queryText = edit; }
	void setTableWidget(QTableWidget* edit) { this->tableWidget = edit; }

public slots:
	void changed(int index);
	void submit();
	void ContextMenu(QPoint position);

	void copyUserJID();
	void copyUserName();
	void copyCharID();
	void copyCharName();
	void copyGuildID();
	void copyGuildName();
	void copyHWID();
	void copyIP();
	void copyLastLogin();

	void BanHWID();
	void UnBanHWID();
	void BanIP();
	void UnBanIP();

	void AllBanHWID();
	void AllUnBanHWID();
	void AllBanIP();
	void AllUnBanIP();

private:
	query_type currentType;
	QLineEdit* queryText;
	QTableWidget* tableWidget;
	void setClipboard(const QString& text);

	void updateAllRows();
	void updateAllHWIDRows(QString _hwid, bool newValue);
	void updateAllIPRows(QString _ip, bool newValue);
};
