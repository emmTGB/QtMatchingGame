#include "GameModel.h"
#include <cstdlib>
#include <ctime>

GameModel::GameModel(GameMode mode):
    gameStatus(WAITING),
    gameMode(mode)
{
	gameMap = nullptr;
	iconSpecies = 0;
	tLevelNum = tRemainNum = 0;
	score = 0;
	combo = 0;
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
	if (gameMap) delete gameMap;
	gameMap = new int[tLevelNum] { -1 };
	if (hintArr) delete hintArr;
	hintArr = new Point[2];
	for (int i = 0; i < 2; i++) {
		hintArr[i].first = 0;
		hintArr[i].second = 0;
	}

	gameStatus = PLAYING;
	iconSpecies = getIconSpecies();
	score = 0;

	int iconID = 0;
	for (int i = 0; i + 1 < tLevelNum; i += 2) {
		gameMap[i] = iconID;
		gameMap[i + 1] = iconID;
		iconID = ++iconID % iconSpecies;
	}

	shuffle();

	frozened = false;
	paintPoints.clear();
}

GameStatus GameModel::checkGameStatus() {
	return gameStatus;
}

GameLevel GameModel::checkGameLevel() {
	return gameLevel;
}

GameMode GameModel::checkGameMode() {
	return gameMode;
}

bool GameModel::linkTwoTiles(Point& src, Point& dst) {
	if (isCanLink(src, dst)) {
		gameMap[colNum * src.second + src.first] = -1;
		gameMap[colNum * dst.second + dst.first] = -1;
		tRemainNum -= 2;
		score += LINK_SCORE * (1 + std::min(MAX_RATE, COMBO_RATE * combo));
		combo++;
		return true;
	}
	combo = 0;
	return false;
}

bool GameModel::checkFrozen() {
	bool ret = __checkFrozen();
	paintPoints.clear();
	return ret;
}

bool GameModel::__checkFrozen() {
	if (!tLevelNum)
		return true;
	for (int i = 0; i < tLevelNum; ++i) {
		for (int j = i; j < tLevelNum; ++j) {
			Point src(i % colNum, i / colNum), dst(j % colNum, j / colNum);

			if (isCanLink(src, dst)) {
				hintArr = new Point[2]{ src, dst };

				frozened = false;
				return false;
			}
		}
	}

	hintArr = nullptr;
	frozened = true;

	return true;
}

bool GameModel::isWin() {
	return tRemainNum == 0;
}

bool GameModel::isFrozen() {
	return frozened;
}

int* GameModel::getHint() {
	if (hintArr) {
		return new int[2] {hintArr[0].first + colNum * hintArr[0].second,
			hintArr[1].first + colNum * hintArr[1].second};
	}
	else {
		return nullptr;
	}
}

int GameModel::getIdAt(int i) {
	return gameMap[i];
}

int GameModel::getLevelNum() {
	return tLevelNum;
}

int GameModel::getCurScore() {
	return score;
}

bool GameModel::isExistAt(int i) {
	return gameMap[i] != -1;
}

void GameModel::setGameStatus(GameStatus status){
    gameStatus = status;
}

void GameModel::punishment(int sc) {
	combo = 0;
	score -= sc;
}

void GameModel::settleScore(int remainTime) {
	if (gameStatus == WIN)
		score += WIN_BONUS;
	score += remainTime / 1000 * TIME_RATE;
	if (score < 0)
		score = 0;
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

	if (gameMap[dst.first + src.second * colNum] == -1 
		&& canLinkDirectly(src, cornerPoint1) 
		&& canLinkDirectly(dst, cornerPoint1)) {
		if (!frozened) {
			paintPoints.clear();
			paintPoints.push_back(src);
			paintPoints.push_back(cornerPoint1);
			paintPoints.push_back(dst);
		}
		return true;
	}
	if (gameMap[cornerPoint2.first + cornerPoint2.second * colNum] == -1
		&& canLinkDirectly(src, cornerPoint2) 
		&& canLinkDirectly(dst, cornerPoint2)) {
		if (!frozened) {
			paintPoints.clear();
			paintPoints.push_back(src);
			paintPoints.push_back(cornerPoint2);
			paintPoints.push_back(dst);
		}
		return true;
	}

	return false;
}

bool GameModel::_twoCornerX(const Point& src, const Point& dst, int x) {
	if (x != src.first && x != dst.first) {
		Point p1 = std::make_pair(x, src.second);
		Point p2 = std::make_pair(x, dst.second);
		bool flag = false;
		if (x == -1 || x == colNum) {
			if (canLinkDirectly(src, p1) && canLinkDirectly(p2, dst))
				flag = true;
		}
		else if (gameMap[x + colNum * src.second] == -1
			&& gameMap[x + colNum * dst.second] == -1
			&& canLinkDirectly(src, p1)
			&& canLinkDirectly(p1, p2)
			&& canLinkDirectly(p2, dst)) {
			flag = true;
		}
		if (flag && !frozened) {
			paintPoints.clear();
			paintPoints.push_back(src);
			paintPoints.push_back(p1);
			paintPoints.push_back(p2);
			paintPoints.push_back(dst);
			return true;
		}
	}
	return false;
}

bool GameModel::_twoCornerY(const Point& src, const Point& dst, int y) {
	if (y != src.second && y != dst.second) {
		Point p1 = std::make_pair(src.first, y);
		Point p2 = std::make_pair(dst.first, y);
		
		bool flag = false;
		if (y == -1 || y == rowNum) {
			if (canLinkDirectly(src, p1) && canLinkDirectly(p2, dst))
				flag = true;
		}
		else if (gameMap[src.first + colNum * y] == -1
			&& gameMap[dst.first + colNum * y] == -1
			&& canLinkDirectly(src, p1)
			&& canLinkDirectly(p1, p2)
			&& canLinkDirectly(p2, dst)) {
			flag = true;
		}
		if (flag && !frozened) {
			paintPoints.clear();
			paintPoints.push_back(src);
			paintPoints.push_back(p1);
			paintPoints.push_back(p2);
			paintPoints.push_back(dst);
			return true;
		}
	}
	return false;
}

bool GameModel::canLinkWithTwoCorner(const Point& src, const Point& dst) {

	int leftX = std::min(src.first, dst.first);
	int rightX = std::max(src.first, dst.first);

	for (int x = leftX + 1; x < rightX; x++)
		if (_twoCornerX(src, dst, x))
			return true;
	for (int x = leftX - 1; x >= -1; x--)
		if (_twoCornerX(src, dst, x))
			return true;
	for (int x = rightX + 1; x <= colNum; x++)
		if (_twoCornerX(src, dst, x))
			return true;

	int topY = std::min(src.second, dst.second);
	int bottomY = std::max(src.second, dst.second);

	for (int y = topY + 1; y < bottomY; y++)
		if (_twoCornerY(src, dst, y))
			return true;
	for (int y = topY - 1; y >= -1; y--)
		if (_twoCornerY(src, dst, y))
			return true;
	for (int y = bottomY + 1; y <= rowNum; y++)
		if (_twoCornerY(src, dst, y))
			return true;

	return false;
}

void GameModel::shuffle() {
	srand((unsigned)time(0));
	for (int i = 0; i < tLevelNum; ++i) {
		int randomID = rand() % tLevelNum;
		std::swap(gameMap[i], gameMap[randomID]);
	}
}

void GameModel::adapt() {
	if (getIconSpecies() < iconSpecies) {
		iconSpecies = getIconSpecies();
		for (int i = 0; i < tLevelNum; ++i) {
			gameMap[i] %= iconSpecies;
		}
	}
}
