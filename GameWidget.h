#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtWidgets\QMainWindow>
#include "QtMatchingGame.h"
#include "ui_GameWidget.h"
#include "IconButton.h"
#include "GameModel.h"
#include <QCloseEvent>

namespace Ui {
class GameWidget;
}

class GameWidget : public QMainWindow
{
    Q_OBJECT

protected:
    QtMatchingGame* mainQ;

    GameModel* game;
    IconButton** buttonImage;
    QTimer* gameTimer;
    
    IconButton* preIcon, * curIcon;
    bool isLinking;

    QMediaPlayer* audioPlayer;

    virtual void initGame();
    Ui::GameWidgetClass ui;

protected slots:
    void onIconButtonPressed();
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

private:
    void destroyButtons();
};

#endif // GAMEWIDGET_H
