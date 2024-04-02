#include "GameWinDialog.h"

GameWinDialog::GameWinDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

GameWinDialog::~GameWinDialog()
{}

QString GameWinDialog::getWinnerName() {
    return ui.nameLine->text();
}

void GameWinDialog::setScores(int cur, int high) {
	curScore = cur;
	highScore = high;
}
