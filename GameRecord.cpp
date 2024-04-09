#include "GameRecord.h"
#include "GameModel.h"

#include <QSqlError>

QString hostName = "localhost";
QString dbName = "qtdatabase";
QString dbUser = "root";
QString dbPass = "123456";

GameRecord::GameRecord() {
	dbConn = QSqlDatabase::addDatabase("QMYSQL");
	dbConn.setPort(3306);
	dbConn.setHostName(hostName);
	dbConn.setDatabaseName(dbName);
	dbConn.setUserName(dbUser);
	dbConn.setPassword(dbPass);
	qry = QSqlQuery(dbConn);

	qDebug() << hostName;
}

GameRecord::~GameRecord() {
}

void GameRecord::insertRecord(Record* record) {
	if (dbConn.open()) {
		__insertDB(record);
	}
	else {
		qDebug() << dbConn.lastError().text();
	}
	// 
}

void GameRecord::insertRecord(const QString& _id, int _score, QDateTime _recordTime, GameMode _gameMode) {
	insertRecord(new Record(_id, _score, _recordTime, _gameMode));
}

std::vector<Record*>* GameRecord::getFrontRecords(GameMode gameMode) {
	if (dbConn.open()) {
		return __getFrontDB(gameMode);
		qDebug() << 1;
	}
}

Record* GameRecord::getFirstRecord(GameMode gameMode) {
	Record* res = nullptr;
	if (dbConn.open()) {
		res =  __getFirstDB(gameMode);
	}

	if (!res) {
		res = new Record("NULL", -1, QDateTime::currentDateTimeUtc(), gameMode);
	}
	return res;
}

void GameRecord::__insertDB(Record* record) {
	QString sTime = record->recordTime.toString("yyyyMMddhhmmss");

	QString strQ = QString("insert into mgrecord(id, score, recordTime, mode) values('%1', %2, %3, %4)");
	strQ = strQ.arg(record->id)
		.arg(record->score)
		.arg(sTime)
		.arg((int)record->gameMode);
	//QString strQ = QString("insert into mgrecord(id, score, recordTime, mode) values(:id, :score, :recordTime, :mode)");
	qry.prepare(strQ);/*
	qry.bindValue(":id", record->id);
	qry.bindValue(":score", record->score);
	qry.bindValue(":recordTime", record->recordTime);
	qry.bindValue(":mode", record->gameMode);*/
	qry.exec();

	qry.finish();
	dbConn.close();
}

std::vector<Record*>* GameRecord::__getFrontDB(GameMode gameMode) {
	std::vector<Record*>* records = new std::vector<Record*>();

	QString strQ = QString("select * from mgrecord where mode = %1 order by score desc limit 0, 10");
	strQ = strQ.arg((int)gameMode);
	qry.prepare(strQ);
	if (qry.exec()) {
		int i = 0;
		while (qry.next()) {
			records->push_back(new Record(
				qry.value(0).toString(),
				qry.value(1).toInt(),
				qry.value(2).toDateTime(),
				GameMode(qry.value(3).toInt())
			));
		}
	}
	records->push_back(nullptr);

	qry.finish();
	dbConn.close();
	return records;
}

Record* GameRecord::__getFirstDB(GameMode gameMode) {
	Record* record = nullptr;

	QString strQ = QString("select * from mgrecord where mode = %1 order by score desc limit 1");
	strQ = strQ.arg((int)gameMode);
	qry.prepare(strQ);
	if (qry.exec()) {
		int i = 0;
		if (qry.next()) {
			record = new Record();
			record->id = qry.value(0).toString();
			record->score = qry.value(1).toInt();
			record->recordTime = qry.value(2).toDateTime();
			record->gameMode = GameMode(qry.value(3).toInt());
		}
	}

	qry.finish();
	dbConn.close();
	return record;
}
