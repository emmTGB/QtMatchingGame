#include "EntertainModeGame.h"

EntertainModeGame::EntertainModeGame(QWidget* parent, QtMatchingGame* mainq) :
	GameWidget(parent, mainq, ENTERTAIN){
	setWindowTitle("休闲模式");

	enableTimer = false;
	ui.timerBar->hide();
	ui.diyButton_1->setText("全自动");

	botLinking = false;
	botTimer = new QTimer(this);
	botTimer->setInterval(botInterval);
	connect(botTimer, SIGNAL(timeout()), this, SLOT(botLink()));
}

EntertainModeGame::~EntertainModeGame() {
}

void EntertainModeGame::changeGameStatus(GameStatus status) {
	GameWidget::changeGameStatus(status);

	// bot按钮
	if (status > PLAYING) {
		ui.diyButton_1->setEnabled(false);
		if (botTimer->isActive())
			botTimer->stop();
	}
	else
		ui.diyButton_1->setEnabled(true);
}

void EntertainModeGame::botLink() {
	int* arr = game->getHint();
	if (arr == nullptr) {
		buttonBling(ui.shuffleBtn, sBtnHoverStyle);
		on_shuffleBtn_clicked();
	}
	else {
		preIcon = buttonImage[arr[0]];
		curIcon = buttonImage[arr[1]];
		buttonBling(buttonImage[arr[0]], tBtnHoverStyle);
		buttonBling(buttonImage[arr[1]], tBtnHoverStyle);
		game->linkTwoTiles(arr[0], arr[1]);
		isLinking = true;
		curLineColor = new QColor(rand() % 96 + 64, rand() % 96 + 64, rand() % 96 + 64);
		GameWidget::update();
		QTimer::singleShot(botInterval - 400, this, SLOT(afterLink()));
	}
	if (game->checkGameStatus() > PLAYING) {
		if (botTimer->isActive()) {
			botTimer->stop();
		}
	}
}

void EntertainModeGame::on_IconButton_Pressed() {
	if (botLinking) {
		return;
	}
	else {
		GameWidget::on_IconButton_Pressed();
	}
}

void EntertainModeGame::on_diyButton_1_clicked(){
	if (botTimer->isActive()) {
		botTimer->stop();
		botLinking = false;
	}
	else {
		botTimer->start();
		botLinking = true;
	}
}