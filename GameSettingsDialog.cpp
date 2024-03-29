#include "GameSettingsDialog.h"
#include "gamewidget.h"

#include <QButtonGroup>
#include <QPixmap>

GameSettingsDialog::GameSettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.presentationWidget->setStyleSheet("#presentationWidget{border-image:url("
		+ getBackground()
		+");}");

	QImage iconEle, iconMsk;
	iconEle.load(getIconElements());
	iconMsk.load(getIconMasks());
	iconEle.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	iconEle.setAlphaChannel(iconMsk);
	QPixmap pix = QPixmap::fromImage(iconEle).copy(0, 0, tIconSize, tIconSize);
	QIcon ico(pix);
	ui.presentationBtn->setStyleSheet(tDefaultStyle);
	ui.presentationBtn->setIcon(ico);
	ui.presentationBtn->setIconSize(pix.size());

	res = theme;

	block = new QButtonGroup(this);
    block->addButton(ui.rCXK, 0);
	block->addButton(ui.rFRT, 1);
	block->addButton(ui.rMHW, 2);
	connect(block, SIGNAL(idClicked(int)), this, SLOT(handleRadio(int)));

	switch (res) {
	case 0:
		ui.rCXK->setChecked(1);
		break;
	case 1:
		ui.rFRT->setChecked(1);
		break;
	case 2:
		ui.rMHW->setChecked(1);
		break;
	default:
		break;
	}
}

GameSettingsDialog::~GameSettingsDialog()
{
	delete block;
}

GameTheme GameSettingsDialog::getRes() {
	return res;
}

void GameSettingsDialog::handleRadio(int i) {
	res = (GameTheme)i;
	qDebug() << i;
	ui.presentationWidget->setStyleSheet("#presentationWidget{border-image:url("
		+ getBackground(res)
		+ ");}");

	QImage iconEle, iconMsk;
	iconEle.load(getIconElements(res));
	iconMsk.load(getIconMasks(res));
	iconEle.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	iconEle.setAlphaChannel(iconMsk);
	QPixmap pix = QPixmap::fromImage(iconEle).copy(0, 0, tIconSize, tIconSize);
	QIcon ico(pix);
	ui.presentationBtn->setStyleSheet(tDefaultStyle);
	ui.presentationBtn->setIcon(ico);
	ui.presentationBtn->setIconSize(pix.size());
}