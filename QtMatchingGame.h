#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMatchingGame.h"
#include <qmediaplayer.h>

class IconButton1 : QPushButton {
    Q_OBJECT

public:
    IconButton1(QMainWindow* parent = Q_NULLPTR) :
        QPushButton(parent),
        xID(-1),
        yID(-1){
    }
    int xID, yID;
};

class QtMatchingGame : public QMainWindow
{
    Q_OBJECT

public:
    QtMatchingGame(QWidget *parent = nullptr);
    ~QtMatchingGame();

private slots:
    void on_pushBasicMode_clicked();

    void on_pushEntertainMode_clicked();

private:
    Ui::QtMatchingGameClass ui;
};
