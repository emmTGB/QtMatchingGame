#include "EntertainModeGame.h"

EntertainModeGame::EntertainModeGame(QWidget* parent, QtMatchingGame* mainq) :
	GameWidget(parent, mainq){
	setWindowTitle("休闲模式");

	enableTimer = false;
	ui.timerBar->hide();
}

EntertainModeGame::~EntertainModeGame() {
}

void EntertainModeGame::on_diyButton_1_clicked(){

}