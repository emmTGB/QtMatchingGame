#pragma once

#include <QDialog>
#include "ui_LevelSelectDialog.h"

class LevelSelectDialog : public QDialog
{
	Q_OBJECT

public:
	LevelSelectDialog(QWidget *parent = nullptr, int curId = 0);
	~LevelSelectDialog();

	int getRes();

private slots:
	void handleRadio(int i);

private:
	int res = 0;
	QButtonGroup* block;

	Ui::LevelSelectDialogClass ui;
};
