#pragma once
#include "GameWidget.h"
class EntertainModeGame :
    public GameWidget {
public:
    EntertainModeGame(QWidget* parent = nullptr, QtMatchingGame* mainq = nullptr);
    ~EntertainModeGame();
};

