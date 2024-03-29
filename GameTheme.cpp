#include "GameTheme.h"

GameTheme theme = CXK;

int getIconSpecies(GameTheme _theme) {
	return ICON_SPC[_theme];
}

QString getIconElements(GameTheme _theme) {
	return ELE_THMEME[_theme];
}

QString getIconMasks(GameTheme _theme) {
	return MSK_THMEME[_theme];
}

QString getBackground(GameTheme _theme) {
	return BG_THEME[_theme];
}