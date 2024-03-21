#include "GameModel.h"
#include <cstdlib>
#include <ctime>

GameModel::GameModel(GameMode mode):
    gameStatus(WAITING),
    gameMode(mode)
{
	gameMap = nullptr;
	tLevelNum = tRemainNum = 0;
	colNum = 0;
	rowNum = 0;
	frozened = false;
	gameLevel = EASY;
	hintArr = nullptr;
}

GameModel::~GameModel() {
	if (gameMap) {
		free(gameMap);
		free(hintArr);
		gameMap = NULL;
	}
}

void GameModel::startGame() {
	colNum = (int)(MAX_COL * levelRate[gameLevel]);
	rowNum = (int)(MAX_ROW * levelRate[gameLevel]);
	if ((colNum * rowNum) % 2) ++colNum;
	tLevelNum = tRemainNum = colNum * rowNum;
	gameMap = new int[tLevelNum] { -1 };
	hintArr = new Point[2];
	for (int i = 0; i < 2; i++) {
		hintArr[i].first = 0;
		hintArr[i].second = 0;
	}

	gameStatus = PLAYING;

	int iconID = 0;
	for (int i = 0; i + 1 < tLevelNum; i += 2) {
		gameMap[i] = iconID;
		gameMap[i + 1] = iconID;
		iconID = ++iconID % MAX_ICON;
	}

	shuffle();

	frozened = false;
	paintPoints.clear();

}

int* GameModel::getGameMap() {
	return gameMap;
}

GameStatus GameModel::checkGameStatus() {
	return gameStatus;
}

bool GameModel::linkTwoTiles(Point& src, Point& dst) {
	if (isCanLink(src, dst)) {
		gameMap[colNum * src.second + src.first] = -1;
		gameMap[colNum * dst.second + dst.first] = -1;
		return true;
	}
	return false;
}

bool GameModel::isFrozen() {
	if (!tLevelNum)
		return true;
	for (int i = 0; i < tLevelNum; ++i) {
		for (int j = 0; j < tLevelNum; ++j) {
			Point src(i % colNum, i / colNum), dst(j % colNum, j / colNum);

			frozened = true;
			if (isCanLink(src, dst)) {
				hintArr[0] = src;
				hintArr[1] = dst;

				frozened = false;

				return false;
			}
		}
	}

	frozened = true;

	return true;
}

bool GameModel::isWin() {
	return tRemainNum == 0;
}

Point* GameModel::getHint() {
	return hintArr;
}

int GameModel::getIdAt(int i) {
	return gameMap[i];
}

int GameModel::getLevelNum() {
	return tLevelNum;
}

bool GameModel::isExistAt(int i) {
	return gameMap[i] != -1;
}

void GameModel::setGameStatus(GameStatus status){
    gameStatus = status;
}

void GameModel::setGameLevel(GameLevel level) {
	gameLevel = level;
}

bool GameModel::isCanLink(Point& src, Point& dst) {
	if (gameMap[src.first + src.second * colNum] == -1 || gameMap[dst.first + dst.second * colNum] == -1)
		return false;
	if (src.first == dst.first && src.second == dst.second)
		return false;
	if (gameMap[src.first + src.second * colNum] != gameMap[dst.first + dst.second * colNum])
		return false;
	
	if (canLinkDirectly(src, dst))
		return true;
	if (canLinkWithOneCorner(src, dst))
		return true;
	if (canLinkWithTwoCorner(src, dst))
		return true;

	return false;
}

bool GameModel::canLinkDirectly(const Point& src, const Point& dst) {

	if (src.first == dst.first) {
		int dstY = std::max(src.second, dst.second);
		int srcY = std::min(src.second, dst.second);
		for (int y = srcY + 1; y < dstY; ++y) {
			if (gameMap[src.first + y * colNum] != -1)
				return false;
		}

		if (!frozened) {
			Point p1(src.first, srcY), p2(dst.first, dstY);
			paintPoints.clear();
			paintPoints.push_back(p1);
			paintPoints.push_back(p2);
		}

		return true;
	}

	if (src.second == dst.second) {
		int dstX = std::max(src.first, dst.first);
		int srcX = std::min(src.first, dst.first);
		for (int x = srcX + 1; x < dstX; ++x) {
			if (gameMap[x + src.second * colNum] != -1)
				return false;
		}

		if (!frozened) {
			Point p1(srcX, src.second), p2(dstX, dst.second);
			paintPoints.clear();
			paintPoints.push_back(p1);
			paintPoints.push_back(p2);
		}

		return true;
	}

	return false;
}

bool GameModel::canLinkWithOneCorner(const Point& src, const Point& dst) {
	Point cornerPoint1(dst.first, src.second), cornerPoint2(src.first, dst.second);

	if (gameMap[cornerPoint1.first + cornerPoint1.second * colNum] == -1 
		&& canLinkDirectly(src, cornerPoint1) 
		&& canLinkDirectly(dst, cornerPoint1)) {
		if (!frozened) {
			paintPoints.clear();
			paintPoints.push_back(src);
			paintPoints.push_back(dst);
			paintPoints.push_back(cornerPoint1);
		}
	}
	if (gameMap[cornerPoint2.first + cornerPoint2.second * colNum] == -1
		&& canLinkDirectly(src, cornerPoint2) 
		&& canLinkDirectly(dst, cornerPoint2)) {
		if (!frozened) {
			paintPoints.clear();
			paintPoints.push_back(src);
			paintPoints.push_back(dst);
			paintPoints.push_back(cornerPoint2);
		}
	}

	return false;
}

bool GameModel::canLinkWithTwoCorner(const Point& src, const Point& dst) {
	Point p1, p2;

	//if((src.first == 0 || (canLinkDirectly(src, std::make_pair(-1, src.second))))
	//	&& dst.first == 0 || (canLinkDirectly(dst, std::make_pair(-1, dst.second))))
	//	if (!frozened) {
	//		p1 = std::make_pair(-1, src.second);
	//		p2 = std::make_pair(-1, dst.second);
	//	}
	//if((src.first == MAX_COL - 1 || (canLinkDirectly(src, std::make_pair(MAX_COL, src.second))))
	//	&& dst.first == MAX_COL - 1 || (canLinkDirectly(dst, std::make_pair(MAX_COL, dst.second))))
	//	if (!frozened) {
	//		p1 = std::make_pair(MAX_COL, src.second);
	//		p2 = std::make_pair(MAX_COL, dst.second);
	//	}
	//if((src.second == 0 || (canLinkDirectly(src, std::make_pair(src.first, -1))))
	//	&& dst.second == 0 || (canLinkDirectly(dst, std::make_pair(dst.first, -1))))
	//	if (!frozened) {
	//		p1 = std::make_pair(src.first, -1);
	//		p2 = std::make_pair(src.first, -1);
	//	}
	//if((src.second == MAX_COL - 1 || (canLinkDirectly(src, std::make_pair(src.first, MAX_COL))))
	//	&& dst.second == MAX_COL - 1 || (canLinkDirectly(dst, std::make_pair(dst.first, MAX_COL))))
	//	if (!frozened) {
	//		p1 = std::make_pair(src.first, MAX_COL);
	//		p2 = std::make_pair(src.first, MAX_COL);
	//	}

	for (int x = -1; x < colNum; ++x) {
		if (x != src.first && x != dst.first) {
			p1 = std::make_pair(x, src.second);
			p2 = std::make_pair(x, dst.second);
			if (((x == -1 || x == colNum) && (canLinkDirectly(src, p1) && canLinkDirectly(p2, dst)))
				|| (gameMap[x + colNum * src.second] == -1
					&& canLinkDirectly(src, p1)
					&& canLinkDirectly(p1, p2)
					&& canLinkDirectly(p2, dst))) {
				if(!frozened) {
					paintPoints.clear();
					paintPoints.push_back(src);
					paintPoints.push_back(p1);
					paintPoints.push_back(p2);
					paintPoints.push_back(dst);
				}
				return true;
			}
		}
	}

	for (int y = -1; y <= rowNum; ++y) {
		if (y != src.second && y != dst.second) {
			p1 = std::make_pair(src.first, y);
			p2 = std::make_pair(dst.first, y);
			if (((y == -1 || y == colNum) && (canLinkDirectly(src, p1)) && (canLinkDirectly(p2, dst)))
				|| (gameMap[src.first + colNum * y] == -1
					&& canLinkDirectly(src, p1)
					&& canLinkDirectly(p1, p2)
					&& canLinkDirectly(p2, dst))) {
				if (!frozened) {
					paintPoints.clear();
					paintPoints.push_back(src);
					paintPoints.push_back(p1);
					paintPoints.push_back(p2);
					paintPoints.push_back(dst);
				}
				return true;
			}
		}
	}

	return false;
}

void GameModel::shuffle() {
	srand((unsigned)time(0));
	for (int i = 0; i < tLevelNum; ++i) {
		int randomID = rand() % tLevelNum;
		std::swap(gameMap[i], gameMap[randomID]);
	}
}
