#pragma once
#include<vector>

#include "Global.h"

const int MAX_COL = 16;
const int MAX_ROW = 16;

const int MAX_ICON = 10;

const int LINK_SCORE = 50;
const double MAX_RATE = 0.3;
const double COMBO_RATE = 0.05;

enum GameStatus {
	WAITING,
	PLAYING,
	PAUSE,
	WIN,
	OVER
};

enum GameLevel {
	EASY = 0,
	NORMAL,
	HARD
};
const double levelRate[] = { 0.3, 0.5, 1.0 };

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
	bool linkTwoTiles(Point& src, Point& dst);
	bool checkFrozen();
	bool isWin();
	bool isFrozen();
	bool isExistAt(int i);
	int getIdAt(int i);
	int* getHint();
	int getLevelNum();

	void setGameLevel(GameLevel level);
    void setGameStatus(GameStatus status);

	void shuffle();

private:
	GameLevel gameLevel;
	GameStatus gameStatus;
	GameMode gameMode;

	int tLevelNum;
	int tRemainNum;
	int score;
	int combo;
	int* gameMap;
	Point* hintArr;

	bool frozened;

	bool isCanLink(Point& src, Point& dst);
	
	bool canLinkDirectly(const Point& src, const Point& dst);
	bool canLinkWithOneCorner(const Point& src, const Point& dst);
	bool canLinkWithTwoCorner(const Point& src, const Point& dst);

	bool _twoCornerX(const Point& src, const Point& dst, int x);
	bool _twoCornerY(const Point& src, const Point& dst, int y);
};

