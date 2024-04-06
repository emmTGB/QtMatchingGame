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

	qry = nullptr;

	qDebug() << hostName;
}

GameRecord::~GameRecord() {
}

void GameRecord::insertRecord(Record* record) {
	if (dbConn.open()) {
		qDebug() << 1;
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

Record** GameRecord::getFrontRecords(GameMode gameMode) {
	if (dbConn.open()) {
		return __getFrontDB(gameMode);
	}
}

Record* GameRecord::getFirstRecord(GameMode gameMode) {
	if (dbConn.open()) {
		return __getFirstDB(gameMode);
	}
}

void GameRecord::__insertDB(Record* record) {
	QString sTime = record->recordTime.toString("yyyyMMddhhmmss");

	QSqlQuery qry(dbConn);
	QString strQ = QString("insert into mgrecord(id, score, recordTime, mode) values(%1, %2, %3, %4)");
	strQ.arg(record->id);
	strQ.arg(record->score);
	strQ.arg(sTime);
	strQ.arg((int)record->gameMode);
	qry.prepare(strQ);
	qry.exec();
	qry.finish();
	dbConn.close();
}

Record** GameRecord::__getFrontDB(GameMode gameMode) {
	Record* records[10] = { nullptr };

	QSqlQuery qry(dbConn);
	QString strQ = QString("select * from mgrecord where mode = %1 orderby score asc limit 0, 9");
	strQ.arg(gameMode);
	qry.prepare(strQ);
	if (qry.exec()) {
		int i = 0;
		while (qry.next()) {
			records[i] = new Record();
			records[i]->id = qry.value(0).toString();
			records[i]->score = qry.value(1).toInt();
			records[i]->recordTime = qry.value(2).toDateTime();
			records[i]->gameMode = GameMode(qry.value(3).toInt());
			++i;
		}
	}

	qry.finish();
	dbConn.close();
	return records;
}

Record* GameRecord::__getFirstDB(GameMode gameMode) {
	Record* record = nullptr;

	QSqlQuery qry(dbConn); 
	QString strQ = QString("select * from mgrecord where mode = %1 orderby score asc limit 0");
	strQ.arg(gameMode);
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
