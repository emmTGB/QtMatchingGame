#pragma once

#include<qpushbutton.h>
#include <QtWidgets\QMainWindow>

class IconButton :
    public QPushButton {
    Q_OBJECT

public:
    IconButton(QMainWindow* parent = Q_NULLPTR) :
        QPushButton(parent),
        xID(-1),
        yID(-1),
        imageID(-1) {
    }
    int xID, yID;
    int imageID;
};

