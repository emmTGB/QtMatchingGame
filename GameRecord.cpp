#include "GameRecord.h"

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

void GameRecord::insertRecord(const QString& _id, int _score, QDateTime _recordTime) {
	insertRecord(new Record(_id, _score, _recordTime));
}

Record** GameRecord::getFrontRecords() {
	if (dbConn.open()) {
		return __getFrontDB();
	}
}

void GameRecord::__insertDB(Record* record) {
	QString sTime = record->recordTime.toString();

	QSqlQuery qry(dbConn);
	qry.prepare("insert into mgrecord(id, score, recordTime) values(:id, :score, :recordTime)");
	qry.bindValue(":id", record->id);
	qry.bindValue(":score", record->score);
	qry.bindValue(":recordTime", record->recordTime.toString("yyyyMMddhhmmss"));
	qry.exec();
	dbConn.close();
}

Record** GameRecord::__getFrontDB() {
	Record* records[10] = { nullptr };

	QSqlQuery qry(dbConn);
	if (qry.exec("select * from mgrecord orderby score asc limit 0, 9")) {
		int i = 0;
		while (qry.next()) {
			records[i] = new Record();
			records[i]->id = qry.value(0).toString();
			records[i]->score = qry.value(1).toInt();
			records[i]->recordTime = qry.value(2).toDateTime();
			++i;
		}
	}

	return records;
}


