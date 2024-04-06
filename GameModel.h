#pragma once
#include<vector>

#include "Global.h"
#include "GameTheme.h"

const int MAX_COL = 18;
const int MAX_ROW = 16;

const int WIN_BONUS = 100;
const int LINK_SCORE = 50;
const double MAX_RATE = 0.3;
const double COMBO_RATE = 0.05;
const double TIME_RATE = 0.05;
const int SFL_PUNISH[] = { 0, 10, 30 };
const int HNT_PUNISH[] = { 10, 30, 50 };

enum GameStatus {
	PLAYING,
	PAUSE,
	WAITING,
	WIN,
	OVER
};

enum GameLevel {
	EASY = 0,
	NORMAL,
	HARD
};
const double levelRate[] = { 0.3, 0.5, 0.8 };

enum GameMode {
	BASIC,
	ENTERTAIN,
	STAGE
};

class GameModel {
public:
	GameModel(GameMode mode = GameMode::BASIC);
	~GameModel();

	int colNum, rowNum;

	std::vector<Point> paintPoints;

	void startGame();
	GameStatus checkGameStatus();
	GameLevel checkGameLevel();
	GameMode checkGameMode();
	bool linkTwoTiles(Point& src, Point& dst);
	bool checkFrozen();
	bool isWin();
	bool isFrozen();
	bool isExistAt(int i);

	int getIdAt(int i);
	int* getHint();
	int getLevelNum();
	int getCurScore();

	void setGameLevel(GameLevel level);
    void setGameStatus(GameStatus status);
	void punishment(int sc);
	void settleScore(int remainTime);

	void shuffle();

	void adapt();

private:
	GameLevel gameLevel;
	GameStatus gameStatus;
	GameMode gameMode;

	int iconSpecies;
	int tLevelNum;
	int tRemainNum;
	int score;
	int combo;
	int* gameMap;
	Point* hintArr;

	bool frozened;

	bool __checkFrozen();

	bool isCanLink(Point& src, Point& dst);
	
	bool canLinkDirectly(const Point& src, const Point& dst);
	bool canLinkWithOneCorner(const Point& src, const Point& dst);
	bool canLinkWithTwoCorner(const Point& src, const Point& dst);

	bool _twoCornerX(const Point& src, const Point& dst, int x);
	bool _twoCornerY(const Point& src, const Point& dst, int y);
};