#pragma once
#include<vector>

const int MAX_COL = 16;
const int MAX_ROW = 16;

const int MAX_ICON = 20;

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

typedef std::pair<int, int> Point;

class GameModel {
public:
	GameModel(GameMode mode = GameMode::BASIC);
	~GameModel();

	int colNum, rowNum;

	std::vector<Point> paintPoints;

	void startGame();
	int* getGameMap();
	GameStatus checkGameStatus();
	bool linkTwoTiles(Point& src, Point& dst);
	bool isFrozen();
	bool isWin();
	bool isExistAt(int i);
	int getIdAt(int i);
	Point* getHint();
	int getLevelNum();

	void setGameLevel(GameLevel level);
    void setGameStatus(GameStatus status);

private:
	GameLevel gameLevel;
	GameStatus gameStatus;
	GameMode gameMode;

	int tLevelNum;
	int tRemainNum;
	int* gameMap;
	Point* hintArr;

	bool frozened;

	bool isCanLink(Point& src, Point& dst);
	
	bool canLinkDirectly(const Point& src, const Point& dst);
	bool canLinkWithOneCorner(const Point& src, const Point& dst);
	bool canLinkWithTwoCorner(const Point& src, const Point& dst);

	void shuffle();
};
