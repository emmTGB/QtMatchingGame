#pragma once

#include <QDialog>
#include "ui_GameWinDialog.h"

class GameWinDialog : public QDialog
{
	Q_OBJECT

public:
	GameWinDialog(QWidget *parent = nullptr);
	~GameWinDialog();

	QString getWinnerName();
	void setScores(int cur, int high = -1);

private:
	int curScore, highScore;

	Ui::GameWinDialogClass ui;
};
