#include "gamewidget.h"
#include "LevelSelectDialog.h"
#include "GameSettingsDialog.h"

#include <QPainter>
#include<qtimer.h>
#include <QMessageBox>

void GameWidget::initGame() {
    game->startGame();
    
    while (game->checkFrozen())
        game->shuffle();

    levelNum = game->getLevelNum();
    buttonImage = new IconButton * [levelNum] { nullptr };
    for (int i = 0; i < levelNum; ++i) {
        buttonImage[i] = new IconButton(this);
        buttonImage[i]->setStyleSheet(tDefaultStyle);
        connect(buttonImage[i], SIGNAL(pressed()), this, SLOT(on_IconButton_Pressed()));
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
            //buttonImage[i]->setText(QString::number(id));
            buttonImage[i]->setIcon(tPix);
            buttonImage[i]->setIconSize(tPix.size());
        }
        else {
            buttonImage[i]->hide();
        }
    }
}

void GameWidget::enableIconButtons() {
    for (int i = 0; i < levelNum; ++i) {
        buttonImage[i]->setEnabled(true);
    }
}

void GameWidget::disableIconButtons() {
    for (int i = 0; i < levelNum; ++i)
        buttonImage[i]->setEnabled(false);
}

GameWidget::GameWidget(QWidget *parent, QtMatchingGame* mainQ, GameMode mode)
    : QMainWindow(parent), mainQ(mainQ)
{
    ui.setupUi(this);
    ui.centralWidget->installEventFilter(this);
    ui.centralWidget->setStyleSheet("#centralWidget{border-image: url("
        + getBackground()
        + ");}");
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);

    game = new GameModel(mode);
    buttonImage = nullptr;
    gameTimer = nullptr;
    preIcon = curIcon = nullptr;
    isLinking = false;
    audioPlayer = nullptr;

    ui.startBtn->setEnabled(true);
    ui.pauseBtn->setEnabled(false);
    ui.hintBtn->setEnabled(false);
    ui.shuffleBtn->setEnabled(false);
    ui.levelBtn->setEnabled(true);
    ui.diyButton_1->setEnabled(false);

    loadIcons();

    setAttribute(Qt::WA_DeleteOnClose, true);
}

GameWidget::~GameWidget()
{
    destroyTButtons();
    delete game;
}

void GameWidget::closeEvent(QCloseEvent* event) {
    mainQ->show();
    destroyTButtons();
}

bool GameWidget::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Paint) {
        QPainter painter(ui.centralWidget);
        QPen pen;
        QColor color(rand() % 96 + 64, rand() % 96 + 64, rand() % 96 + 64);
        pen.setColor(color);
        pen.setWidth(5);
        painter.setPen(pen);

        for (int i = 1; i < game->paintPoints.size(); ++i) {
            Point p1 = game->paintPoints[i - 1];
            Point p2 = game->paintPoints[i];

            QPoint pos1(tLeftMargin + p1.first * tIconSize + tIconSize / 2,
                tTopMargin + p1.second * tIconSize + tIconSize / 2);
            QPoint pos2(tLeftMargin + p2.first * tIconSize + tIconSize / 2,
                tTopMargin + p2.second * tIconSize + tIconSize / 2);

            painter.drawLine(pos1, pos2);
            qDebug() << "paint" << QString::number(pos1.x()) << QString::number(pos2.y());;
        }
        game->paintPoints.clear();
        return true;
    }
    else {
        return QMainWindow::eventFilter(watched, event);
    }
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
                update();
                isLinking = true;

                QTimer::singleShot(tLinkingTimerDelay, this, SLOT(afterLink()));
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
    ui.showWidget->setStyleSheet("");

    enableIconButtons();
}

void GameWidget::on_pauseBtn_clicked()
{
    switch (game->checkGameStatus()) {
    case PLAYING:
        game->setGameStatus(PAUSE);
        ui.pauseBtn->setText("继续游戏");
        ui.levelBtn->setEnabled(true);
        ui.startBtn->setEnabled(true);
        ui.shuffleBtn->setEnabled(false);
        ui.hintBtn->setEnabled(false);
        disableIconButtons();
        break;
    case PAUSE:
        game->setGameStatus(PLAYING);
        ui.pauseBtn->setText("暂停游戏");
        ui.levelBtn->setEnabled(false);
        ui.startBtn->setEnabled(false);
        ui.shuffleBtn->setEnabled(true);
        ui.hintBtn->setEnabled(true);
        enableIconButtons();
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
    if (buttonImage)
        for (int i = 0; i < levelNum; ++i)
            buttonImage[i]->setStyleSheet(tDefaultStyle);
    preIcon = nullptr;
    curIcon = nullptr;
    isLinking = false;
}

void GameWidget::afterLink() {
    game->checkFrozen();
    preIcon->setStyleSheet(tReleasedStyle);
    curIcon->setStyleSheet(tReleasedStyle);
    preIcon->hide();
    curIcon->hide();
    preIcon = curIcon = nullptr;

    update(); 
    if (game->isWin()) {
        ui.startBtn->setEnabled(true);
        ui.pauseBtn->setEnabled(false);
        ui.hintBtn->setEnabled(false);
        ui.shuffleBtn->setEnabled(false);
        ui.showWidget->setStyleSheet("#showWidget{border-image:url("
            + WIN_PIC
            + ");}");

        QMessageBox::information(this, "great", "you win");
    }

    isLinking = false;
}

void GameWidget::on_shuffleBtn_clicked()
{
    if (isLinking)
        return;
    if (!game->isFrozen())
        // 未发现僵局，则对重排动作执行分数惩罚
        ;
    releaseTButtons();
    do {
        game->shuffle();
    } while (game->checkFrozen());
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
            disableIconButtons();
        }
    }
    delete lsd;
}

void GameWidget::loadIcons() {
    QImage iconEle, iconMsk;
    iconEle.load(getIconElements());
    iconMsk.load(getIconMasks());
    
    iconEle.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    iconEle.setAlphaChannel(iconMsk);

    tIconMap = QPixmap::fromImage(iconEle);
}

void GameWidget::on_hintBtn_clicked()
{
    if (isLinking)
        return;
    int* hintArr = game->getHint();
    if (hintArr == nullptr) {
        // TODO：未发现提示，想一种醒目的提示方法
        // 暂定为按钮闪烁
    }
    else {
        buttonImage[hintArr[0]]->setStyleSheet(tHintStyle);
        buttonImage[hintArr[1]]->setStyleSheet(tHintStyle);
        qDebug() << hintArr[0] << hintArr[1];
        //需要复位逻辑
    }
}

void GameWidget::on_pushButton_clicked()
{
    GameSettingsDialog* gsd = new GameSettingsDialog(this);
    int ref = gsd->exec();
    if (ref == QDialog::Accepted) {
        if (gsd->getRes() != theme) {
            theme = gsd->getRes();
            ui.centralWidget->setStyleSheet("#centralWidget{border-image: url("
                + getBackground()
                + ");}");
            game->adapt();
            loadIcons();
            paintTiles();
        }
    }
    delete gsd;
}