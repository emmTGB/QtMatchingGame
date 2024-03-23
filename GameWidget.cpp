#include "gamewidget.h"
#include "LevelSelectDialog.h"

#include <QPainter>
#include <QMessageBox>

void GameWidget::initGame() {
    game->startGame();
    
    levelNum = game->getLevelNum();
    buttonImage = new IconButton * [levelNum] { nullptr };
    for (int i = 0; i < levelNum; ++i) {
        buttonImage[i] = new IconButton(this);
        buttonImage[i]->setStyleSheet(tDefaultStyle);
    }
    paintTiles();

    // 进度条

    // 计时器

    isLinking = false;

    //播放音乐
}

void GameWidget::paintTiles() {
    /*QPixmap tIconMap;
    if (!tIconMap.load(":/QtMatchingGame/res/fruit_element.bmp"))
        return;*//*
    int pWidth = tIconMap.width();
    int pHeight = tIconMap.height();*/

    for (int i = 0; i < levelNum; ++i) {
        buttonImage[i]->setGeometry(tLeftMargin + (i % game->colNum) * tIconSize, tTopMargin + (i / game->colNum) * tIconSize, tIconSize, tIconSize);
        buttonImage[i]->setPos(i % game->colNum, i / game->colNum);
        buttonImage[i]->imageID = game->getIdAt(i);
        buttonImage[i]->show();
        if (game->isExistAt(i)) {
            int id = game->getIdAt(i);
            //截取对应的icon
            QPixmap tPix = tIconMap.copy(0, id * tIconSize, tIconSize, tIconSize);
            QIcon tIcon(tPix);
            buttonImage[i]->setIcon(tPix);
            buttonImage[i]->setIconSize(tPix.size());
            connect(buttonImage[i], SIGNAL(pressed()), this, SLOT(on_IconButton_Pressed()));
        }
        else {
            buttonImage[i]->hide();
        }
    }
}

GameWidget::GameWidget(QWidget *parent, QtMatchingGame* mainQ, GameMode mode)
    : QMainWindow(parent), mainQ(mainQ)
{
    ui.setupUi(this);
    game = new GameModel(mode);
    overlay = new OverlayWidget(this);
    buttonImage = nullptr;
    gameTimer = nullptr;
    preIcon = curIcon = nullptr;
    isLinking = false;
    audioPlayer = nullptr;

    loadIcons();

    setAttribute(Qt::WA_DeleteOnClose, true);
}

GameWidget::~GameWidget()
{
    destroyTButtons();
    delete game;
    delete overlay;
}

void GameWidget::closeEvent(QCloseEvent* event) {
    mainQ->show();
    destroyTButtons();
}

void GameWidget::on_IconButton_Pressed() {
    if (isLinking) {
        return;
    }

    curIcon = dynamic_cast<IconButton*>(sender());
    
    if (preIcon == nullptr) {
        curIcon->setStyleSheet(tClickedStyle);
        preIcon = curIcon;
    }
    else {
        if (preIcon != curIcon) {
            curIcon->setStyleSheet(tClickedStyle);
            if (game->linkTwoTiles(preIcon->posID, curIcon->posID)) {
                isLinking = true;
                update();
            }
            else {
                curIcon->setStyleSheet(tReleasedStyle);
                preIcon->setStyleSheet(tReleasedStyle);
                preIcon = nullptr;
            }
        }
        else {
            curIcon->setStyleSheet(tReleasedStyle);
            preIcon = nullptr;
        }
    }
}

void GameWidget::on_startBtn_clicked()
{
    if(game->checkGameStatus() == WAITING){
        initGame();
        ui.startBtn->setEnabled(false);
        ui.startBtn->setText("重新开始");
    }else{
        // 需要摧毁当前按钮数组
        destroyTButtons();
        ui.pauseBtn->setText("暂停游戏");
        initGame();
        ui.startBtn->setEnabled(false);
    }
    ui.levelBtn->setEnabled(false);
    ui.pauseBtn->setEnabled(true);
    ui.shuffleBtn->setEnabled(true);
    ui.hintBtn->setEnabled(true);
}

void GameWidget::on_pauseBtn_clicked()
{
    switch (game->checkGameStatus()) {
    case PLAYING:
        game->setGameStatus(PAUSE);
        ui.pauseBtn->setText("继续游戏");
        ui.levelBtn->setEnabled(true);
        ui.startBtn->setEnabled(true);
        break;
    case PAUSE:
        game->setGameStatus(PLAYING);
        ui.pauseBtn->setText("暂停游戏");
        ui.levelBtn->setEnabled(false);
        ui.startBtn->setEnabled(false);
        break;
    default:
        break;
    }
}

void GameWidget::destroyTButtons() {
    if (buttonImage) {
        for (int i = 0; i < levelNum; ++i) {
            delete buttonImage[i];
        }
        delete[] buttonImage;
    }
    buttonImage = nullptr;
}

void GameWidget::releaseTButtons() {
    if (preIcon) {
        preIcon->setStyleSheet(tReleasedStyle);
        preIcon = nullptr;
    }
    if (curIcon) {
        curIcon->setStyleSheet(tReleasedStyle);
        curIcon = nullptr;
    }
    isLinking = false;
}

void GameWidget::on_shuffleBtn_clicked()
{
    releaseTButtons();
    game->shuffle();
    paintTiles();
}

void GameWidget::on_levelBtn_clicked()
{
    LevelSelectDialog* lsd = new LevelSelectDialog(this, game->checkGameLevel());
    int ref = lsd->exec();
    if (ref == QDialog::Accepted) {
        GameLevel level = GameLevel(lsd->getRes());
        if (game->checkGameLevel() != level) {
            game->setGameLevel(level);
            game->setGameStatus(PAUSE);
            ui.pauseBtn->setEnabled(false);
            ui.shuffleBtn->setEnabled(false);
            ui.hintBtn->setEnabled(false);
            releaseTButtons();
        }
    }
    delete lsd;
}

void GameWidget::loadIcons() {
    QImage iconEle, iconMsk;
    iconEle.load(":/QtMatchingGame/res/fruit_element.bmp");
    iconMsk.load(":/QtMatchingGame/res/fruit_mask.bmp");
    
    iconEle.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    iconEle.setAlphaChannel(iconMsk);

    tIconMap = QPixmap::fromImage(iconEle);
}

