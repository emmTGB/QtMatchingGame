#pragma once

#include <QWidget>
#include <QDateTime>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "GameModel.h"

extern QString hostName;
extern QString dbName;
extern QString dbUser;
extern QString dbPass;

struct Record {
	Record(const QString& _id, int _score, QDateTime _recordTime, GameMode _gameMode) :
		id(_id), score(_score), recordTime(_recordTime), gameMode(_gameMode) {}
	Record():
		id(""), score(-1), recordTime(QDateTime::currentDateTime()), gameMode(BASIC) {}
	QString id;
	int score;
	QDateTime recordTime;
	GameMode gameMode;
};

class GameRecord {
public:
	GameRecord();
	~GameRecord();

	void insertRecord(const QString& _id, int _score, QDateTime _recordTime, GameMode _gameMode);
	void insertRecord(Record* record);
	Record** getFrontRecords(GameMode gameMode);
	Record* getFirstRecord(GameMode gameMode);

private:
	void __insertDB(Record* record);
	Record** __getFrontDB(GameMode gameMode);
	Record* __getFirstDB(GameMode gameMode);

	bool sqlUsable = true;
	QSqlDatabase dbConn;
	QSqlQuery* qry;
};

