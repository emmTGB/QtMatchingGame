#pragma once

#include <QDialog>
#include "ui_GameRankDialog.h"

class GameRankDialog : public QDialog
{
	Q_OBJECT

public:
	GameRankDialog(QWidget *parent = nullptr);
	~GameRankDialog();

private:
	Ui::GameRankDialogClass ui;
};
