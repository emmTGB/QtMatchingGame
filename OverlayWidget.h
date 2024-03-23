#pragma once

#include <QWidget>

class OverlayWidget : public QWidget
{
	Q_OBJECT

public:
	OverlayWidget(QWidget *parent = nullptr);
	~OverlayWidget();

protected:
	void paintEvent(QPaintEvent* event);
};
