#pragma once

#include <QWidget>
#include <QDateTime>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>

extern QString hostName;
extern QString dbName;
extern QString dbUser;
extern QString dbPass;

struct Record {
	Record(const QString& _id = "miku", int _score = 0, QDateTime _recordTime = QDateTime(QDate(2007, 8, 31), QTime(3, 9, 1))) :
		id(_id), score(_score), recordTime(_recordTime) {}
	QString id;
	int score;
	QDateTime recordTime;
};

class GameRecord {
public:
	GameRecord();
	~GameRecord();

	void insertRecord(const QString& _id = "miku", int _score = 0, QDateTime _recordTime = QDateTime(QDate(2007, 8, 31), QTime(3, 9, 1)));
	void insertRecord(Record* record);
	Record** getFrontRecords();

private:
	void __insertDB(Record* record);
	Record** __getFrontDB();

	bool sqlUsable = true;
	QSqlDatabase dbConn;
};

