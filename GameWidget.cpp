#include "gamewidget.h"

#include <QMessageBox>

const int tIconSize = 40;
const int tTopMargin = 50;
const int tLeftMargin = 50;

void GameWidget::initGame() {
    game->startGame();
    
    int levelNum = game->getLevelNum();
    buttonImage = new IconButton * [levelNum] {nullptr};
    for (int i = 0; i < levelNum; ++i) {
        buttonImage[i] = new IconButton(this);
        buttonImage[i]->setGeometry(tLeftMargin + (i % game->colNum) * tIconSize, tTopMargin + (i / game->colNum) * tIconSize, tIconSize, tIconSize);
        buttonImage[i]->xID = i % game->colNum;
        buttonImage[i]->yID = i / game->colNum;
        buttonImage[i]->imageID = game->getIdAt(i);
        buttonImage[i]->show();
        buttonImage[i]->setText(QString::number(buttonImage[i]->imageID));
        if (game->isExistAt(i)) {
            QPixmap iconPix;
            QString fileString;
            QIcon icon(iconPix);
            // 未完成

            connect(buttonImage[i], SIGNAL(pressed()), this, SLOT(onIconButtonPressed()));
        }
        else {
            buttonImage[i]->hide();
        }
    }

    // 进度条

    // 计时器

    isLinking = false;

    //播放音乐
}

GameWidget::GameWidget(QWidget *parent, QtMatchingGame* mainQ, GameMode mode)
    : QMainWindow(parent), mainQ(mainQ)
{
    ui.setupUi(this);
    game = new GameModel(mode);
    buttonImage = nullptr;
    gameTimer = nullptr;
    preIcon = curIcon = nullptr;
    isLinking = false;
    audioPlayer = nullptr;
}

GameWidget::~GameWidget()
{
    destroyButtons();
}

void GameWidget::closeEvent(QCloseEvent* event) {
    mainQ->show();
}

void GameWidget::onIconButtonPressed() {
}


void GameWidget::on_startBtn_clicked()
{
    if(game->checkGameStatus() == WAITING){
        initGame();
        ui.startBtn->setEnabled(false);
        ui.startBtn->setText("重新开始");
    }else{
        // 需要摧毁当前按钮数组
        destroyButtons();
        initGame();
        ui.startBtn->setEnabled(false);
    }
}


void GameWidget::on_pauseBtn_clicked()
{
    switch (game->checkGameStatus()) {
    case PLAYING:
        game->setGameStatus(PAUSE);
        ui.pauseBtn->setText("继续游戏");
        ui.startBtn->setEnabled(true);
        break;
    case PAUSE:
        game->setGameStatus(PLAYING);
        ui.pauseBtn->setText("暂停游戏");
        ui.startBtn->setEnabled(false);
        break;
    default:
        break;
    }
}

void GameWidget::destroyButtons() {
    if (buttonImage) {
        for (int i = 0; i < game->getLevelNum(); ++i) {
            delete buttonImage[i];
        }
        delete[] buttonImage;
    }
    buttonImage = nullptr;
}
