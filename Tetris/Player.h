#pragma once

#include "Blocks.h"

class Player {

	Tetromino currentBlock;
	Tetromino nextBlock;
	Board board;
	int score;
	int rowClears;
	bool gameOver;

	int bag[7] = { 0,1,2,3,4,5,6 };
	int bagIndex;

protected:

	int pDrawOffset;
	int elapsedTicks;
	GameConfig::PLAYER_TYPES type;
	GameConfig::PLAYERS_ID player;
	void makeMove(GameConfig::MOVES move);

private:

	virtual void calculateMoves(Board& board, Tetromino& cBlock) {}
	void getNextBlock();
	Tetromino getRandomBlock();
	void getCompletedRows();
	void embedBlockInBoard();
	void drawNextBlock();
	void printScore();

public:

	Player(GameConfig::PLAYERS_ID id, GameConfig::PLAYER_TYPES pType);
	int getDrawOffset() const { return pDrawOffset; }
	int getScore() const { return score; }
	bool isGameOver() const { return gameOver; }
	void setNewGame(int dOffset);
	void loadGame();
	void moveLeft();
	void moveRight();
	void moveDown();
	void rotate(GameConfig::ROTATION_DIR dir);
	void dropBlock();
	virtual void checkInput(const char& key) {}
	virtual ~Player() {}

};