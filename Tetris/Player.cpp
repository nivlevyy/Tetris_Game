#include "Player.h"

Player::Player(GameConfig::PLAYERS_ID id, GameConfig::PLAYER_TYPES pType) {

	type = pType;
	player = id;
	rowClears = 0;
	score = 0;
	pDrawOffset = 0;
	bagIndex = 0;
	elapsedTicks = 0;
	gameOver = false;

}

void Player::setNewGame(int dOffset) { //reset stats, board and blocks

	std::random_shuffle(&bag[0], &bag[7]);

	pDrawOffset = dOffset;
	gameOver = false;
	score = 0;
	board.setNewBoard();
	currentBlock = getRandomBlock();
	nextBlock = getRandomBlock();
	calculateMoves(board, currentBlock);

}

void Player::loadGame() { //draws player's game board

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(pDrawOffset + 5, 18);
	std::cout << "P" << (int)player + 1;
	gotoxy(pDrawOffset + 14, 1);
	std::cout << "Next";
	gotoxy(pDrawOffset + 14, 2);
	std::cout << "____";

	board.printBoard(pDrawOffset);
	currentBlock.drawBlock();
	drawNextBlock();

}

void Player::moveLeft() { //move player block to the left
	
	if (currentBlock.validMove(board, -1, 0, pDrawOffset))
		currentBlock.moveBlock(-1, 0, true);

}

void Player::moveRight() { //move player block to the right

	if (currentBlock.validMove(board, 1, 0, pDrawOffset))
		currentBlock.moveBlock(1, 0, true);

}

void Player::moveDown() { //moves player block down

	if (currentBlock.validMove(board, 0, 1, pDrawOffset))
		currentBlock.moveBlock(0, 1, true);
	else
		embedBlockInBoard();

}

void Player::rotate(GameConfig::ROTATION_DIR dir) { //rotate player block according to direction

	currentBlock.rotateBlock(dir);

	if (currentBlock.validMove(board, 0, 0, pDrawOffset)) 
		currentBlock.moveBlock(0, 0, true); //no collision, valid rotation

}

void Player::dropBlock() { //'drop' a player's block by moving down continuously

	while (currentBlock.validMove(board, 0, 1, pDrawOffset)) //while no collision
		currentBlock.moveBlock(0, 1, true);

	embedBlockInBoard(); //collided - embed block in board

}

void Player::embedBlockInBoard() { //embed a block in the board and get new block

	if (currentBlock.getBlockID() == (int)GameConfig::BLOCK_IDS::bombBlock) {
		board.explodeBomb(currentBlock.getShapePointPosY(0), currentBlock.getShapePointPosX(0) - pDrawOffset, pDrawOffset, true);
		board.printBoard(pDrawOffset);
	}
	else {
		//get shape's position relative to board array
		//and set the cell as current blockID
		for (int i = 0; i < 4; i++) //
			board.setNonEmptySpace(currentBlock.getShapePointPosY(i), currentBlock.getShapePointPosX(i) - pDrawOffset, currentBlock.getBlockID());
		getCompletedRows();
	}
	
	score += 5;
	printScore();
	getNextBlock();

	//if new block can't 'spawn'
	//GAME OVER
	if (!currentBlock.validMove(board, 0, 0, pDrawOffset)) {
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //discard any remaining inputs from game
		gameOver = true;
		return;
	}

	elapsedTicks = 0;
	currentBlock.drawBlock();
	drawNextBlock();

}

void Player::getCompletedRows() { //checks for completed rows in a player's board

	int numRowsCleared;

	numRowsCleared = board.checkCompletedRows();

	if (numRowsCleared > 0) {
		rowClears += numRowsCleared;
		board.printBoard(pDrawOffset); //print updated board
		if (numRowsCleared == 1)
			score += 100;
		else if (numRowsCleared == 2)
			score += 250;
		else if (numRowsCleared == 3)
			score += 500;
		else if (numRowsCleared >= 4)
			score += 1000;
	}

}

Tetromino Player::getRandomBlock() { //gets random block for player from tetrominoes array

	Tetromino newBlock;
	
	if (rand() % 20 == 1) //5% for bomb block
		newBlock = GameConfig::tetrominoes[(int)GameConfig::BLOCK_IDS::bombBlock - 1];
		else {
		newBlock = GameConfig::tetrominoes[bag[bagIndex++]];
		if (bagIndex == 7) { //re-shuffle bag after going through all of it
			std::random_shuffle(&bag[0], &bag[7]);
			bagIndex = 0;
		}
	}

	//set block's default origin points
	//account for drawing offset of player's game
	newBlock.setOriginPoints(pDrawOffset + GameConfig::defBlockXOrigin, GameConfig::defBlockYOrigin);

	return newBlock;

}

void Player::getNextBlock() { //get next block for player

	currentBlock = nextBlock;
	calculateMoves(board, currentBlock);
	nextBlock = getRandomBlock();

}

void Player::makeMove(GameConfig::MOVES move) { //move block according to move type

	switch (move) {
	case GameConfig::MOVES::LEFT:
		moveLeft();
		break;
	case GameConfig::MOVES::RIGHT:
		moveRight();
		break;
	case GameConfig::MOVES::DROP:
		dropBlock();
		break;
	case GameConfig::MOVES::RCW:
		rotate(GameConfig::ROTATION_DIR::CW);
		break;
	case GameConfig::MOVES::RCCW:
		rotate(GameConfig::ROTATION_DIR::CCW);
		break;
	case GameConfig::MOVES::DOWN:
	case GameConfig::MOVES::NONE:
	default:
		break;
	}

}

void Player::printScore() { //print player's score

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(pDrawOffset + 6, 20);
	std::cout << score;

}

void Player::drawNextBlock() { //draw player's next block

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	//erase previous block in 'Next' window
	for (int i = 0; i < 4; i++) {
		gotoxy(pDrawOffset + 14, 3 + i);
		std::cout << "    ";
	}

	//set new points for drawing
	//in proper position and reset
	nextBlock.setOriginPoints(pDrawOffset + 14, 4);
	nextBlock.drawBlock();
	nextBlock.setOriginPoints(pDrawOffset + GameConfig::defBlockXOrigin, GameConfig::defBlockYOrigin);

}