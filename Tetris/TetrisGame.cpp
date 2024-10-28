#include "TetrisGame.h"

TetrisGame::TetrisGame() {

	pausedGame = false;
	gameEnded = false;

}

void TetrisGame::setNewGame(const bool& twoPlayers, const GameConfig::MODES& mode, const GameConfig::DIFFICULTY& diff) { //start a new game

	gameEnded = false;

	clrscr();
	std::cout << "Starting a new game." << std::endl;
	Sleep(1000);

	delete player1;
	delete player2;

	//create proper player types and assign difficulty if needed
	if (twoPlayers) {
		switch (mode) {
		case GameConfig::MODES::HumanVSHuman:
			player1 = new Human(GameConfig::player1);
			player2 = new Human(GameConfig::player2);
			break;
		case GameConfig::MODES::HumanVSComputer:
			player1 = new Human(GameConfig::player1);
			player2 = new Computer(GameConfig::player2, diff);
			break;
		case GameConfig::MODES::ComputerVSComputer:
			player1 = new Computer(GameConfig::player1, diff);
			player2 = new Computer(GameConfig::player2, diff);
			break;

		}
	}
	else //single-player mode
		player1 = new Human(GameConfig::player1);

	//set new game parameters for players
	player1->setNewGame(0);
	if (twoPlayers)
		player2->setNewGame(20);

	drawGame(twoPlayers);

}

void TetrisGame::resumeGame(const bool& twoPlayers) { //resume a paused game

	if (!pausedGame) //in case called without a paused game
		setNewGame(twoPlayers, GameConfig::MODES::HumanVSHuman, GameConfig::DIFFICULTY::NONE); //default game mode
	else {
		clrscr();
		std::cout << "Resuming game." << std::endl;
		Sleep(1000);
		drawGame(twoPlayers);
	}

}

void TetrisGame::drawGame(const bool& twoPlayers) { //draw game's current state

	clrscr();

	int p1DOffset, p2DOffset;

	//print player's game
	//by calling loadGame
	player1->loadGame();
	p1DOffset = player1->getDrawOffset();
	if (twoPlayers) {
		player2->loadGame();
		p2DOffset = player2->getDrawOffset();
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	//print two player score template
	if (twoPlayers) {
		gotoxy(p1DOffset, 19);
		std::cout << "--------------------------------" << std::endl;
		gotoxy(p1DOffset, 20);
		std::cout << "SCORE:" << player1->getScore();
		gotoxy(p2DOffset, 20);
		std::cout << "SCORE:" << player2->getScore();
		gotoxy(p1DOffset, 21);
		std::cout << "--------------------------------";
	}
	else { //one player score template
		gotoxy(p1DOffset, 19);
		std::cout << "------------";
		gotoxy(p1DOffset, 20);
		std::cout << "SCORE:" << player1->getScore();
		gotoxy(p1DOffset, 21);
		std::cout << "------------";
	}

	playGame(twoPlayers);

}

void TetrisGame::playGame(const bool& twoPlayers) { //game loop - get inputs and game state

	pausedGame = false;

	char key;

	while (!pausedGame && !gameEnded) {
		key = NULL;
		if (_kbhit()) {
			key = _getch();
			if (key == GameConfig::ESC) {
				pausedGame = true;
				continue;
			}
			key = tolower(key);
		}
		//check if input is part of players' controls
		player1->checkInput(key);
		if (twoPlayers)
			player2->checkInput(key);

		//simulates an update time of 750ms
		//50ms between inputs
		Sleep(GameConfig::gameTiming / 15);

		if (player1->isGameOver() || (twoPlayers && player2->isGameOver())) //check if game over
			gameEnded = true;

	}

	if (gameEnded) //end game screen
		endGameRes(twoPlayers);
	//else paused game
}

void TetrisGame::endGameRes(const bool& twoPlayers) { //end game and declare winner / tie

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(player1->getDrawOffset(), GameConfig::BOARD_HEIGHT + 4);
	std::cout << "GAME OVER";

	//decide winner or tie for two player mode
	//ignore if single-player
	if (twoPlayers) {
		if (player1->isGameOver() && player2->isGameOver()) { //both players lost at same time
			if (player1->getScore() == player2->getScore())
				std::cout << " - It's a tie!";
			else //check whose score is higher 
				player1->getScore() > player2->getScore() ? std::cout << " - Player 1 wins!" : std::cout << " - Player 2 wins!";
		}
		else if (player1->isGameOver()) //player 1 lost
			std::cout << " - Player 2 wins!";
		else //player 2 lost
			std::cout << " - Player 1 wins!";
	}
	std::cout << std::endl;

	gotoxy(player1->getDrawOffset(), GameConfig::BOARD_HEIGHT + 5);
	int i, len;
	twoPlayers ? len = 32 : len = 12;
	for (i = 0; i < len; i++) {
		std::cout << "-";
		Sleep(40);
	}

	delete player1;
	if (twoPlayers)
		delete player2;
	player1 = player2 = nullptr;

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //discard any remaining inputs from game
	std::cout << std::endl << std::endl << "(*) Press any key to go back to the main menu.";
	//wait for keyboard input
	if (_getch())
		return;

}

