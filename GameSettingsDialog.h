#pragma once

#include <QDialog>
#include "ui_GameSettingsDialog.h"
#include "GameTheme.h"

class GameSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	GameSettingsDialog(QWidget *parent = nullptr);
	~GameSettingsDialog();

	GameTheme getRes();

private slots:
	void handleRadio(int i);

private:
	GameTheme res;
	QButtonGroup* block;

	Ui::GameSettingsDialogClass ui;
};
