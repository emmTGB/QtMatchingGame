#include "OverlayWidget.h"
#include <qpainter.h>

OverlayWidget::OverlayWidget(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

OverlayWidget::~OverlayWidget()
{}

void OverlayWidget::paintEvent(QPaintEvent * event) {
	QPainter painter(this);
	painter.fillRect(rect(), QColor(0, 0, 0, 100));
}
