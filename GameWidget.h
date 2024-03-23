#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtWidgets\QMainWindow>
#include "QtMatchingGame.h"
#include "ui_GameWidget.h"
#include "IconButton.h"
#include "GameModel.h"
#include <QCloseEvent>
#include "OverlayWidget.h"

const int tIconSize = 40;
const int tTopMargin = 50;
const int tLeftMargin = 50;

const QString tDefaultStyle = "background: transparent";
const QString tReleasedStyle = tDefaultStyle + "; ";
const QString tClickedStyle = tDefaultStyle + ";background-color: rgba(255, 255, 12, 161);";

class GameWidget : public QMainWindow
{
    Q_OBJECT

protected:
    QtMatchingGame* mainQ;

    int levelNum = -1;

    GameModel* game;
    IconButton** buttonImage;
    QTimer* gameTimer;
    
    IconButton* preIcon, * curIcon;
    bool isLinking;

    QMediaPlayer* audioPlayer;
    
    OverlayWidget* overlay;

    virtual void initGame();
    void paintTiles();
    void putOverlay();

    Ui::GameWidgetClass ui;

protected slots:
    void on_IconButton_Pressed();
    // void gameTimerEvent();
    // void handleLinkEffect();
    // void on_hintBtn_clicked();
    // void on_robotBtn_clicked();

public:
    explicit GameWidget(QWidget *parent = nullptr, QtMatchingGame* mainQ = nullptr, GameMode mode = BASIC);
    ~GameWidget();

    void closeEvent(QCloseEvent* event);

private slots:
    void on_startBtn_clicked();

    void on_pauseBtn_clicked();

    void on_shuffleBtn_clicked();

    void on_levelBtn_clicked();

private:
    QPixmap tIconMap;

    void loadIcons();
    void destroyTButtons();
    void releaseTButtons();
};

#endif // GAMEWIDGET_H
