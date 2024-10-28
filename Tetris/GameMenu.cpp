#include "GameMenu.h"

void GameMenu::loadingScreen() { //simulate game loading screen

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	clrscr();

	std::cout << " ______        __                         \n";
	std::cout << "/\\__  _\\      /\\ \\__         __           \n";
	std::cout << "\\/_/\\ \\/    __\\ \\ ,_\\  _ __ /\\_\\    ____  \n";
	std::cout << "   \\ \\ \\  /'__`\\ \\ \\/ /\\`'__\\/\\ \\  /',__\\ \n";
	std::cout << "    \\ \\ \\/\\  __/\\ \\ \\_\\ \\ \\/ \\ \\ \\/\\__, `\\ \n";
	std::cout << "     \\ \\_\\ \\____\\\\ \\__\\\\ \\_\\  \\ \\_\\/\\____/ \n";
	std::cout << "      \\/_/\\/____/ \\/__/ \\/_/   \\/_/\\/___/ \n\n";

	for (int i = 0; i < 42; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
		std::cout << ' ';
		if (i == 38)
			Sleep(500);
		else
			Sleep(rand() % 50 + 15);
	}

	displayGameMenu();

}

void GameMenu::displayGameMenu() { //display main-menu 

	TetrisGame theGame;

	while (!exit) {
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //discard any inputs before menu shown
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		clrscr();

		std::cout << "TETRIS GAME" << std::endl << "___________" << std::endl << std::endl;
		if (twoPlayers) {
			std::cout << "(1) - Start a new game: Human VS Human." << std::endl;
			std::cout << "(2) - Start a new game: Human VS Computer." << std::endl;
			std::cout << "(3) - Start a new game: Computer VS Computer." << std::endl;
		}
		else
			std::cout << "(1) - Start a new single-player game." << std::endl;
		if (theGame.isPaused() && !toggled) {
			if (twoPlayers)
				std::cout << "(4) - Continue a paused game." << std::endl;
			else
				std::cout << "(2) - Continue a paused game." << std::endl;
		}
		std::cout << "(6) - Toggle two players  - ";
		twoPlayers ? std::cout << "ON." << std::endl : std::cout << "OFF." << std::endl;
		std::cout << "(7) - Toggle colors - ";
		GameConfig::useColors ? std::cout << "ON." << std::endl : std::cout << "OFF." << std::endl;
		std::cout << "(8) - Display instructions and keys." << std::endl;
		std::cout << "(9) - EXIT." << std::endl;

		getMenuChoice(theGame);
	}

}

void GameMenu::getMenuChoice(TetrisGame& theGame) { //get user's input for menu choice

	bool validChoice = false;
	char keyStroke;

	while (!validChoice) {
		keyStroke = _getch();
		if (twoPlayers) {
			switch (keyStroke) {
			case (char)GameConfig::MENU_MP_OPTIONS::START_HVSH: //human vs human
				validChoice = true;
				versusMode = GameConfig::MODES::HumanVSHuman;
				theGame.setNewGame(twoPlayers, versusMode, GameConfig::DIFFICULTY::NONE);
				break;
			case (char)GameConfig::MENU_MP_OPTIONS::START_HVSC: //human vs computer
				validChoice = true;
				versusMode = GameConfig::MODES::HumanVSComputer;
				setDifficulty();
				if (difficulty == GameConfig::DIFFICULTY::NONE) //'ESC' char was pressed
					continue;
				theGame.setNewGame(twoPlayers, versusMode, difficulty);
				break;
			case (char)GameConfig::MENU_MP_OPTIONS::START_CVSC: //computer vs computer
				validChoice = true;
				versusMode = GameConfig::MODES::ComputerVSComputer;
				setDifficulty();
				if (difficulty == GameConfig::DIFFICULTY::NONE) //'ESC' char was pressed (no difficulty chosen)
					continue;
				theGame.setNewGame(twoPlayers, versusMode, difficulty);
				break;
			case (char)GameConfig::MENU_MP_OPTIONS::RESUME_GAME: //show option to resume if valid
				if (theGame.isPaused() && !toggled) { //paused and no change to toggle
					validChoice = true;
					theGame.resumeGame(twoPlayers);
					break; //will set toggledMode to false
				}
				else {
					std::cout << "Invalid choice. Try again." << std::endl;
					continue;
				}
			}
		}
		else { //single-player mode
			switch (keyStroke) {
			case (char)GameConfig::MENU_SP_OPTIONS::START_GAME:
				validChoice = true;
				versusMode = GameConfig::MODES::HumanVSNothing;
				difficulty = GameConfig::DIFFICULTY::NONE;
				theGame.setNewGame(twoPlayers, versusMode, difficulty);
				break;
			case (char)GameConfig::MENU_SP_OPTIONS::RESUME_GAME:
				if (theGame.isPaused() && !toggled) { //paused and no change to toggle
					validChoice = true;
					theGame.resumeGame(twoPlayers);
					break; //will set toggledMode to false
				}
				else {
					std::cout << "Invalid choice. Try again." << std::endl;
					continue;
				}
			}
		}
		if (!validChoice) {
			switch (keyStroke) {
			//will disable / re-enable option to resume depending on paused game mode
			case (char)GameConfig::MENU_GENERAL_OPTIONS::TOGGLE_2PLAYERS:
				validChoice = true;
				twoPlayers = !twoPlayers;
				toggled = !toggled;
				continue;
			case (char)GameConfig::MENU_GENERAL_OPTIONS::TOGGLE_COLORS:
				validChoice = true;
				GameConfig::useColors = !GameConfig::useColors;
				continue;
			case (char)GameConfig::MENU_GENERAL_OPTIONS::INSTRUCTIONS_CONTROLS:
				validChoice = true;
				displayInstructionsAndControls();
				continue;
			case (char)GameConfig::MENU_GENERAL_OPTIONS::EXIT:
				validChoice = true;
				exit = true;
				clrscr();
				std::cout << "Thank you for playing." << std::endl;
				return;
			default: //invalid menu choice
				std::cout << "Invalid choice. Try again." << std::endl;
				continue;
			}
		}

		toggled = false;
	}

}

void GameMenu::displayInstructionsAndControls() { //display instructions and controls

	clrscr();

	std::cout << "HOW TO PLAY" << std::endl << "___________" << std::endl << std::endl;
	std::cout << "Your goal in Tetris is to ";
	std::cout << "form complete rows using tetrominoes (blocks)" << std::endl << "as they descend down from the top ";
	std::cout << "of the board." << std::endl;
	std::cout << "Completed rows get cleared and earns the player extra points." << std::endl;
	std::cout << "1 row cleared - 100 points." << std::endl;
	std::cout << "2 rows cleared - 250 points." << std::endl;
	std::cout << "3 rows cleared - 500 points." << std::endl;
	std::cout << "4 rows cleared (Tetris) - 1000 points." << std::endl;
	std::cout << "The game ends when a player reaches the top of the board." << std::endl;
	std::cout << "(!) Score is factored only in a tie." << std::endl << std::endl;

	std::cout << "CONTROLS" << std::endl << "________" << std::endl << std::endl;
	std::cout << "Player 1" << std::endl << "________" << std::endl << std::endl;
	std::cout << "a / A - move left" << std::endl << "d / D - move right" << std::endl;
	std::cout << "s / S - rotate clockwise" << std::endl << "w / W - rotate counterclockwise" << std::endl;
	std::cout << "x / X - drop" << std::endl << std::endl;

	gotoxy(32, 14);
	std::cout << "________";
	gotoxy(32, 16);
	std::cout << "Player 2";
	gotoxy(32, 17);
	std::cout << "________";
	gotoxy(32, 19);
	std::cout << "j / J - move left";
	gotoxy(32, 20);
	std::cout << "l / L - move right";
	gotoxy(32, 21);
	std::cout << "k / K - rotate clockwise";
	gotoxy(32, 22);
	std::cout << "i / I - rotate counterclockwise";
	gotoxy(32, 23);
	std::cout << "m / M - drop" << std::endl << std::endl;

	std::cout << "(*) Press any key to go back to the main menu.";

	if (_getch())
		return;

}

void GameMenu::setDifficulty() { //set computer's difficulty

	bool valid = false;

	clrscr();
	std::cout << "Choose computer difficulty " << std::endl << "--------------------------" << std::endl << std::endl;
	std::cout << "(a) - Best." << std::endl << "(b) - Good." << std::endl << "(c) - Novice." << std::endl << std::endl;
	std::cout << "(*) Press ESC to go back to the main menu." << std::endl;

	while (!valid) {
		char key = _getch();
		key = tolower(key);
		switch (key) {
		case 'a':
			valid = true;
			difficulty = GameConfig::DIFFICULTY::BEST;
			break;
		case 'b':
			valid = true;
			difficulty = GameConfig::DIFFICULTY::GOOD;
			break;
		case 'c':
			valid = true;
			difficulty = GameConfig::DIFFICULTY::NOVICE;
			break;
		case GameConfig::ESC:
			difficulty = GameConfig::DIFFICULTY::NONE; //set back to default
			return;
		default:
			std::cout << "Invalid choice. Try again." << std::endl;
			break;
		}
	}


}