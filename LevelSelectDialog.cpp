#include "LevelSelectDialog.h"

#include<qbuttongroup.h>

LevelSelectDialog::LevelSelectDialog(QWidget *parent, int curId)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	block = new QButtonGroup(this);
	block->addButton(ui.rEasy, 0);
	block->addButton(ui.rNormal, 1);
	block->addButton(ui.rHard, 2);
	connect(block, SIGNAL(idClicked(int)), this, SLOT(handleRadio(int)));

	res = curId;
	switch (curId) {
	case 0:
		ui.rEasy->setChecked(1);
		break;
	case 1:
		ui.rNormal->setChecked(1);
		break;
	case 2:
		ui.rHard->setChecked(1);
		break;
	default:
		break;
	}
}

LevelSelectDialog::~LevelSelectDialog()
{
	delete block;
}

int LevelSelectDialog::getRes() {
	return res;
}

void LevelSelectDialog::handleRadio(int i) {
	res = i;
}
