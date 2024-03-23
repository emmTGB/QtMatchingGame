#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMatchingGame.h"
#include <qmediaplayer.h>

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
