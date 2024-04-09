#pragma once

#include <QDialog>
#include "ui_GameHelpDialog.h"

class GameHelpDialog : public QDialog
{
	Q_OBJECT

public:
	GameHelpDialog(QWidget *parent = nullptr);
	~GameHelpDialog();

private:
	Ui::GameHelpDialogClass ui;
};
