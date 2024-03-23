#include "IconButton.h"

void IconButton::setPos(const Point& pos) {
	this->posID = pos;
}

void IconButton::setPos(int x, int y) {
	posID = std::make_pair(x, y);
}
