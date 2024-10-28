#pragma once

#include "PlayerTypes.h"

class TetrisGame {

	bool pausedGame;
	bool gameEnded;
	Player* player1 = nullptr;
	Player* player2 = nullptr;

private:

	void drawGame(const bool& twoPlayers);
	void playGame(const bool& twoPlayers);
	void endGameRes(const bool& twoPlayers);

public:

	TetrisGame();
	TetrisGame(const TetrisGame& ref) = delete;
	void setNewGame(const bool& twoPlayers, const GameConfig::MODES& mode, const GameConfig::DIFFICULTY& diff);
	void resumeGame(const bool& twoPlayers);
	bool isPaused() const { return pausedGame; }
	TetrisGame& operator=(const TetrisGame& ref) = delete;
	~TetrisGame() { delete player1; delete player2; };

};