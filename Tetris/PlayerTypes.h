#pragma once

#include "Player.h"

using std::vector;
using std::pair;

class Human : public Player {

	char pKeys[5];

public:

	Human(GameConfig::PLAYERS_ID id) : Player(id, GameConfig::HUMAN) { setKeys(); }
	void checkInput(const char& key) override;

private:

	void calculateMoves(Board& board, Tetromino& cBlock) override {}
	void setKeys();

};

class Computer : public Player {

	GameConfig::DIFFICULTY difficulty;
	vector<pair<float, vector<pair<Point, GameConfig::MOVES>>>> possibleRoutes;
	vector<pair<Point, GameConfig::MOVES>> currentMove;

public:

	void checkInput(const char& key) override;
	Computer(GameConfig::PLAYERS_ID id, GameConfig::DIFFICULTY _difficulty) : Player(id, GameConfig::COMPUTER), difficulty(_difficulty) {}

private:

	void addRoute(const float& routeScore, const Point& pos, vector<pair<Point, GameConfig::MOVES>>& route);
	float evaluateRoute(const Tetromino& currentBlock, Board board) const;
	void calculateMoves(Board& board, Tetromino& cBlock) override;
	void goDownAndAdd(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route);
	bool rotateAndFind(Tetromino& block, const Board& board, vector<pair<Point, GameConfig::MOVES>>& route);
	void recursiveFindLeft(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route);
	void recursiveFindRight(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route);
	void findDown(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route);

};