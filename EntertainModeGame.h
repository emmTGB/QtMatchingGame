#pragma once
#include "GameWidget.h"
class EntertainModeGame :
    public GameWidget {
public:
    EntertainModeGame(QWidget* parent = nullptr, QtMatchingGame* mainq = nullptr);
    ~EntertainModeGame();

protected slots:
    // 自定义按钮功能：机器人
    void on_diyButton_1_clicked() override;
};
