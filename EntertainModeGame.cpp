#include "EntertainModeGame.h"

EntertainModeGame::EntertainModeGame(QWidget* parent, QtMatchingGame* mainq) :
	GameWidget(parent, mainq){
	setWindowTitle("休闲模式");

}

EntertainModeGame::~EntertainModeGame() {
}
