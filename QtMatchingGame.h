#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMatchingGame.h"
#include <qmediaplayer.h>

#include "GameTheme.h"
#include "GameRecord.h"

class QtMatchingGame : public QMainWindow
{
    Q_OBJECT

public:
    QtMatchingGame(QWidget *parent = nullptr);
    ~QtMatchingGame();

    GameRecord& getConnect();

private slots:
    void on_pushBasicMode_clicked();

    void on_pushEntertainMode_clicked();

    void on_rankBtn_clicked();

    void on_helpBtn_clicked();

private:
    GameRecord gRecord;
    Ui::QtMatchingGameClass ui;
};
