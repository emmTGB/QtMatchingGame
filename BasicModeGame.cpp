#include "BasicModeGame.h"

BasicModeGame::BasicModeGame(QWidget* parent, QtMatchingGame* mainQ) :
	GameWidget(parent, mainQ) {
	ui.diyButton_1->hide();
}

BasicModeGame::~BasicModeGame() {

}
