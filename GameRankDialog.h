#pragma once

#include "QtMatchingGame.h"

#include <QDialog>
#include <QTableView>
#include <qstandarditemmodel.h>
#include "ui_GameRankDialog.h"

class GameRankDialog : public QDialog
{
	Q_OBJECT

public:
	GameRankDialog(QWidget *parent = nullptr, QtMatchingGame* mainQ = nullptr);
	~GameRankDialog();

private slots:
    void on_tabWidget_tabBarClicked(int index);

private:
	QTableView** tables;
	QStandardItemModel** models;
	QtMatchingGame* mainQ;
	Ui::GameRankDialogClass ui;
};
