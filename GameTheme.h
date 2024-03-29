#ifndef GAMETHEME_H
#define GAMETHEME_H

#include <QWidget>
#include "Global.h"

enum GameTheme {
	CXK = 0,
	FRT,
	MHW
};

const QString BG_THEME[] = {
	BG_CXK,
	BG_FRT,
	BG_MHW
};

const int ICON_SPC[] = {
	ICON_SPC_CXK,
	ICON_SPC_FRT,
	ICON_SPC_MHW
};

const QString ELE_THMEME[] = {
	ELE_CXK,
	ELE_FRT,
	ELE_MHW
};
const QString MSK_THMEME[] = {
	MSK_CXK,
	MSK_FRT,
	MSK_MHW
};

extern GameTheme theme;

int getIconSpecies(GameTheme _theme = theme);

QString getIconElements(GameTheme _theme = theme);

QString getIconMasks(GameTheme _theme = theme);

QString getBackground(GameTheme _theme = theme);

#endif