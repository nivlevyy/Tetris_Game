#pragma once

#include "TetrisGame.h"

class GameMenu {

	bool exit = false;
	bool twoPlayers = true;
	bool toggled = false;
	GameConfig::MODES versusMode = GameConfig::MODES::HumanVSHuman;
	GameConfig::DIFFICULTY difficulty = GameConfig::DIFFICULTY::NONE;

private:

	void displayInstructionsAndControls();
	void getMenuChoice(TetrisGame& theGame);

public:

	void setDifficulty();
	void loadingScreen();
	void displayGameMenu();

};