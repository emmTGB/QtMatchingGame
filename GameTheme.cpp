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

QString getClickSound(GameTheme _theme) {
	return CLK_THEME[_theme];
}

QString getClearSound(GameTheme _theme) {
	return CLR_THEME[_theme];
}

QString getBackgroudMusic(GameTheme _theme) {
	return BGM_THEME[_theme];
}
