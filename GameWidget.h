#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtWidgets\QMainWindow>
#include "QtMatchingGame.h"
#include "ui_GameWidget.h"
#include "IconButton.h"
#include "GameModel.h"
#include <QCloseEvent>
#include <QAudioOutput>

const int tIconSize = 40;
const int tTopMargin = 70;
const int tLeftMargin = 50;

const QString tDefaultStyle = "background: transparent";
const QString tReleasedStyle = tDefaultStyle + "; ";
const QString tClickedStyle = tDefaultStyle + ";background-color: rgba(255, 255, 12, 161);border-width: 2px;border-color: rgb(255, 32, 64);";
const QString tHintStyle = tDefaultStyle + "; background-color: rgba(128, 0, 255, 96);";

const int tLinkingTimerDelay = 700;

const int gameTimerTotal = 5 * 60 * 1000;
const int gameTimerInterval = 300;

class GameWidget : public QMainWindow
{
    Q_OBJECT

protected:
    QtMatchingGame* mainQ;

    int levelNum = -1;

    GameModel* game;
    IconButton** buttonImage;
    
    IconButton* preIcon, * curIcon;
    QColor* curLineColor;
    bool isLinking;
    bool enableTimer;

    QMediaPlayer* audioPlayer;

    virtual void initGame();
    void pauseGame();
    void continueGame();
    void endGame(GameStatus status);

    void paintTiles();
    void enableIconButtons();
    void disableIconButtons();
    //void paintEvent(QPaintEvent* event)Q_DECL_OVERRIDE;
    bool eventFilter(QObject* watched, QEvent* event) Q_DECL_OVERRIDE;

    void playBGM();
    void stopBGM();
    void playClkEfe();
    void playClrEfe();

    void startTimer();
    void pauseTimer();

    Ui::GameWidgetClass ui;

protected slots:
    void on_IconButton_Pressed();
    void afterLink();
    void gameTimerEvent();

public:
    explicit GameWidget(QWidget *parent = nullptr, QtMatchingGame* mainQ = nullptr, GameMode mode = BASIC);
    ~GameWidget();

    void closeEvent(QCloseEvent* event);

private slots:
    void on_startBtn_clicked();

    void on_pauseBtn_clicked();

    void on_shuffleBtn_clicked();

    void on_levelBtn_clicked();

    void on_hintBtn_clicked();

    void on_settingsBtn_clicked();

private:
    QPixmap tIconMap;
    QMediaPlayer* bgmPlayer, * effectPlayer;
    QAudioOutput* bgmOutput, * effectOutput;
    QTimer* gameTimer;

    void loadIcons();
    void destroyTButtons();
    void releaseTButtons();
};

#endif // GAMEWIDGET_H
