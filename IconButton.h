#pragma once

#include <qpushbutton.h>
#include <QtWidgets\QMainWindow>
#include "Global.h"

class IconButton :
    public QPushButton {
    Q_OBJECT

public:
    IconButton(QMainWindow* parent = Q_NULLPTR) :
        QPushButton(parent),
        imageID(-1) {
    }

    void setPos(const Point& pos);
    void setPos(int x, int y);

    int imageID;
    Point posID;
};

