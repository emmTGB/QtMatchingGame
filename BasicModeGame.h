#pragma once
#include "gamewidget.h"

class BasicModeGame :
    public GameWidget {

public:
    BasicModeGame(QWidget* parent = nullptr, QtMatchingGame* mainQ = nullptr);
    ~BasicModeGame();
};

