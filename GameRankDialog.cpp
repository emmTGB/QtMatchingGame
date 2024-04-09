#include "GameRankDialog.h"
#include "GameRecord.h"

GameRankDialog::GameRankDialog(QWidget* parent, QtMatchingGame* mainQ)
	: QDialog(parent) {
	ui.setupUi(this);

	this->mainQ = mainQ;

	tables = new QTableView * [3] {
		ui.tableView_1,
		ui.tableView_2,
		ui.tableView_3
	};
	models = new QStandardItemModel*[3];

	for (int i = 0; i < 3; ++i) {
		models[i] = new QStandardItemModel();
		QStandardItemModel* model = models[i];
		tables[i]->setModel(model);
		tables[i]->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tables[i]->horizontalHeader()->setStretchLastSection(true);
	}

	on_tabWidget_tabBarClicked(0);
}

GameRankDialog::~GameRankDialog()
{}

void GameRankDialog::on_tabWidget_tabBarClicked(int index)
{
	QStandardItemModel* model = models[index];
	model->clear();
	std::vector<Record*>* records = mainQ->getConnect()->getFrontRecords(GameMode(index));
	int i = 0;
	while (records->at(i) != nullptr) {
		QStringList strL;
		strL << "昵称" << "得分" << "时间";
		model->setHorizontalHeaderLabels(strL);

		QString id, score, time;
		id = records->at(i)->id;
		score = QString::number(records->at(i)->score);
		time = records->at(i)->recordTime.toString();
		model->setItem(i, 0, new QStandardItem(id));
		model->setItem(i, 1, new QStandardItem(score));
		model->setItem(i, 2, new QStandardItem(time));
		i++;
	}
}

