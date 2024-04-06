#include "BasicModeGame.h"

BasicModeGame::BasicModeGame(QWidget* parent, QtMatchingGame* mainQ) :
	GameWidget(parent, mainQ) {
	setWindowTitle("基本模式");
	ui.diyButton_1->hide();

	enableTimer = true;
}

BasicModeGame::~BasicModeGame() {

}
