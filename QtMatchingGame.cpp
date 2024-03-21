#include "QtMatchingGame.h"
#include "BasicModeGame.h"

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

void QtMatchingGame::on_pushBasicMode_clicked()
{
    this->close();
    BasicModeGame *bmg = new BasicModeGame(nullptr, this);
    bmg->show();
}


void QtMatchingGame::on_pushEntertainMode_clicked()
{

}

