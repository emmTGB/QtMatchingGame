#include "QtMatchingGame.h"
#include "BasicModeGame.h"
#include "EntertainModeGame.h"

QtMatchingGame::QtMatchingGame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedWidth(this->width());
    setFixedHeight(this->height());
    
}

QtMatchingGame::~QtMatchingGame()
{
}

GameRecord& QtMatchingGame::getConnect() {
    return gRecord;
}

void QtMatchingGame::on_pushBasicMode_clicked()
{
    this->close();
    BasicModeGame *bmg = new BasicModeGame(nullptr, this);
    bmg->show();
}


void QtMatchingGame::on_pushEntertainMode_clicked()
{
    this->close();
    EntertainModeGame* emg = new EntertainModeGame(nullptr, this);
    emg->show();
}


void QtMatchingGame::on_rankBtn_clicked()
{

}


void QtMatchingGame::on_helpBtn_clicked()
{

}

