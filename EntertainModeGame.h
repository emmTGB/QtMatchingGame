#pragma once
#include "GameWidget.h"
class EntertainModeGame :
    public GameWidget {
public:
    EntertainModeGame(QWidget* parent = nullptr, QtMatchingGame* mainq = nullptr);
    ~EntertainModeGame();

protected slots:
    // �Զ��尴ť���ܣ�������
    void on_diyButton_1_clicked() override;
};
