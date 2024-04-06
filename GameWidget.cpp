#include "gamewidget.h"
#include "LevelSelectDialog.h"
#include "GameSettingsDialog.h"
#include "GameWinDialog.h"

#include <QPainter>
#include<qtimer.h>
#include <QMessageBox>

GameWidget::GameWidget(QWidget* parent, QtMatchingGame* mainQ, GameMode mode)
    : QMainWindow(parent), mainQ(mainQ) {
    ui.setupUi(this);
    ui.centralWidget->installEventFilter(this);
    ui.centralWidget->setStyleSheet("#centralWidget{border-image: url("
        + getBackground()
        + ");}");
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);

    game = new GameModel(mode);
    buttonImage = nullptr;
    preIcon = curIcon = nullptr;
    curLineColor = nullptr;
    isLinking = false;
    enableTimer = true;

    ui.startBtn->setEnabled(true);
    ui.pauseBtn->setEnabled(false);
    ui.hintBtn->setEnabled(false);
    ui.shuffleBtn->setEnabled(false);
    ui.levelBtn->setEnabled(true);
    ui.diyButton_1->setEnabled(false);

    ui.scoreLbl->setText("0");

    ui.timerBar->setMaximum(gameTimerTotal);
    ui.timerBar->setMinimum(0);
    ui.timerBar->setValue(gameTimerTotal);

    bgmOutput = new QAudioOutput(this);
    bgmOutput->setVolume(0.15);
    effectOutput = new QAudioOutput(this);
    effectOutput->setVolume(0.5);

    bgmPlayer = new QMediaPlayer(this);
    bgmPlayer->setAudioOutput(bgmOutput);
    bgmPlayer->setLoops(-1);

    effectPlayer = new QMediaPlayer(this);
    effectPlayer->setAudioOutput(effectOutput);
    effectPlayer->setLoops(1);

    gameTimer = new QTimer(this);
    gameTimer->setInterval(gameTimerInterval);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameTimerEvent()));

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
    if (enableTimer) {
        ui.timerBar->setValue(gameTimerTotal);
        startTimer();
    }

    playBGM();

    isLinking = false;
}

void GameWidget::pauseGame() {
    if (game->checkGameStatus() > PAUSE)
        return;
    game->setGameStatus(PAUSE);
    ui.pauseBtn->setText("继续游戏");
    ui.levelBtn->setEnabled(true);
    ui.startBtn->setEnabled(true);
    ui.shuffleBtn->setEnabled(false);
    ui.hintBtn->setEnabled(false);
    disableIconButtons();
    if (enableTimer)
        pauseTimer();
}

void GameWidget::continueGame() {
    game->setGameStatus(PLAYING);
    ui.pauseBtn->setText("暂停游戏");
    ui.levelBtn->setEnabled(false);
    ui.startBtn->setEnabled(false);
    ui.shuffleBtn->setEnabled(true);
    ui.hintBtn->setEnabled(true);
    enableIconButtons();
    if (enableTimer)
        startTimer();
}

void GameWidget::endGame(GameStatus status) {
    game->setGameStatus(status);
    ui.startBtn->setEnabled(true);
    ui.pauseBtn->setEnabled(false);
    ui.hintBtn->setEnabled(false);
    ui.shuffleBtn->setEnabled(false);
    disableIconButtons();
    stopBGM();
    if (enableTimer)
        pauseTimer();

    game->settleScore(ui.timerBar->value());
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

void GameWidget::playBGM() {
    bgmPlayer->setSource(QUrl(getBackgroudMusic()));
    if (game->checkGameStatus() > PAUSE)
        return;
    bgmPlayer->play();
}

void GameWidget::stopBGM() {
    bgmPlayer->stop();
}

void GameWidget::playClkEfe() {
    effectPlayer->setSource(QUrl(getClickSound()));
    effectPlayer->play();
}

void GameWidget::playClrEfe() {
    effectPlayer->setSource(QUrl(getClearSound()));
    effectPlayer->play();
}

void GameWidget::startTimer() {
    gameTimer->start();
}

void GameWidget::pauseTimer() {
    gameTimer->stop();
}

void GameWidget::buttonBling(QPushButton* button, const QString& hoverStyle) {
    QLabel* blingLabel = new QLabel(this);
    blingLabel->setText("");
    blingLabel->setGeometry(button->geometry());
    blingLabel->setStyleSheet(hoverStyle);
    blingLabel->show();

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(blingLabel);
    effect->setOpacity(1);
    blingLabel->setGraphicsEffect(effect);

    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setDuration(800);
    animation->setLoopCount(2);

    QObject::connect(animation, &QPropertyAnimation::start, [&]() {

        });
    QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
            blingLabel->hide();
            delete blingLabel;
        });

    animation->setKeyValueAt(0, 0);
    animation->setKeyValueAt(0.5, 0.8);
    animation->setKeyValueAt(1, 0);

    animation->start(QAbstractAnimation::KeepWhenStopped);
}

bool GameWidget::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Paint) {
        if (game->paintPoints.size() && curLineColor) {
            QPainter painter(ui.centralWidget);
            QPen pen;
            pen.setColor(*curLineColor);
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
            }
        }
        return true;
    }
    else {
        return QMainWindow::eventFilter(watched, event);
    }
}

void GameWidget::afterLink() {
    game->checkFrozen();
    game->paintPoints.clear();
    preIcon->setStyleSheet(tReleasedStyle);
    curIcon->setStyleSheet(tReleasedStyle);
    preIcon->hide();
    curIcon->hide();
    preIcon = curIcon = nullptr;

    update(); 
    if (game->isWin()) {
        endGame(WIN);

        GameWinDialog* gwd = new GameWinDialog(this);
        gwd->setScores(game->getCurScore(),
            mainQ->getConnect().getFirstRecord(game->checkGameMode())->score); // 
        int ref = gwd->exec();
        if (ref == QDialog::Accepted) {
            mainQ->getConnect().insertRecord(
                gwd->getWinnerName(),
                game->getCurScore(),
                QDateTime::currentDateTime(),
                game->checkGameMode()
            );
        }
        delete gwd;
        //
        ui.showWidget->setStyleSheet("#showWidget{border-image:url("
            + WIN_PIC
            + ");}");
    }

    isLinking = false;
}

void GameWidget::gameTimerEvent() {
    if(ui.timerBar->value() <= 0){
        gameTimer->stop();
        //
        endGame(OVER);
    }
    else{
        ui.timerBar->setValue(ui.timerBar->value() - gameTimerInterval);
    }
    update();
}

void GameWidget::on_IconButton_Pressed() {
    if (isLinking) {
        return;
    }

    curIcon = dynamic_cast<IconButton*>(sender());
    
    if (preIcon == nullptr) {
        curIcon->setStyleSheet(tClickedStyle);
        playClkEfe();
        preIcon = curIcon;
    }
    else {
        if (preIcon != curIcon) {
            curIcon->setStyleSheet(tClickedStyle);
            if (game->linkTwoTiles(preIcon->posID, curIcon->posID)) {
                if (curLineColor) delete curLineColor;
                curLineColor = new QColor(rand() % 96 + 64, rand() % 96 + 64, rand() % 96 + 64);
                update();
                isLinking = true;
                playClrEfe();

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
            playClkEfe();
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
        pauseGame();
        break;
    case PAUSE:
        continueGame();
        break;
    default:
        break;
    }
}

void GameWidget::on_shuffleBtn_clicked()
{
    if (isLinking)
        return;
    if (!game->isFrozen())
        // 未发现僵局，则对重排动作执行分数惩罚
        game->punishment(SFL_PUNISH[game->checkGameLevel()]);
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

void GameWidget::on_hintBtn_clicked()
{
    if (isLinking)
        return;
    int* hintArr = game->getHint();
    if (hintArr == nullptr) {
        // TODO：未发现提示，想一种醒目的提示方法
        // 暂定为按钮闪烁
        buttonBling(ui.shuffleBtn, sBtnHoverStyle);
    }
    else {
        buttonBling(buttonImage[hintArr[0]], tBtnHoverStyle);
        buttonBling(buttonImage[hintArr[1]], tBtnHoverStyle);
    }
    game->punishment(HNT_PUNISH[game->checkGameLevel()]);
}

void GameWidget::on_settingsBtn_clicked()
{
    pauseGame();
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
            playBGM();
            paintTiles();
        }
    }
    delete gsd;
}

void GameWidget::loadIcons() {
    QImage iconEle, iconMsk;
    iconEle.load(getIconElements());
    iconMsk.load(getIconMasks());
    
    iconEle.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    iconEle.setAlphaChannel(iconMsk);

    tIconMap = QPixmap::fromImage(iconEle);
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

void GameWidget::on_diyButton_1_clicked()
{
    return;
}

