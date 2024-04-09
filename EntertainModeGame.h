#pragma once
#include "GameWidget.h"

#include <QTimer>

class EntertainModeGame :
    public GameWidget {
public:
    EntertainModeGame(QWidget* parent = nullptr, QtMatchingGame* mainq = nullptr);
    ~EntertainModeGame();

protected:
    void changeGameStatus(GameStatus status) override;

protected slots:
    // 自定义按钮功能：机器人
    void on_diyButton_1_clicked() override;

    void botLink() override;

    void on_IconButton_Pressed() override;

private:
    QTimer* botTimer;
    const int botInterval = 1600;
    bool botLinking;
};
