#include "GameWinDialog.h"

GameWinDialog::GameWinDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("你赢了！");
}

GameWinDialog::~GameWinDialog()
{}

QString GameWinDialog::getWinnerName() {
	if (ui.nameLine->text() == "") {
		return "无名高手";
	}
	else {
		return ui.nameLine->text();
	}
}

void GameWinDialog::setScores(int cur, int high) {
	if (cur > high) {
		ui.congratulationLbl->setText("恭喜，你是最棒的！");
	}
	else {
		ui.congratulationLbl->setText("恭喜完成游戏");
	}

	ui.curLbl->setText(QString::number(cur));
	ui.hiLbl->setText(QString::number(high));
}
