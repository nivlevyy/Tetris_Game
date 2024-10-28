#include "PlayerTypes.h"

//Human class
//-----------
void Human::setKeys() { //set player's controls according to id

	//assign player keys to index 
	//in correlation to the move type
	if (player == GameConfig::player1) {
		pKeys[0] = GameConfig::LEFT_a;
		pKeys[1] = GameConfig::RIGHT_d;
		pKeys[2] = GameConfig::DROP_x;
		pKeys[3] = GameConfig::ROTATE_CW_s;
		pKeys[4] = GameConfig::ROTATE_CCW_w;
	}
	else {
		pKeys[0] = GameConfig::LEFT_j;
		pKeys[1] = GameConfig::RIGHT_l;
		pKeys[2] = GameConfig::DROP_m;
		pKeys[3] = GameConfig::ROTATE_CW_k;
		pKeys[4] = GameConfig::ROTATE_CCW_i;
	}

}

void Human::checkInput(const char& key) { //check if key is valid input for player

	elapsedTicks++;

	//check if input is valid player key
	for (int i = 0; i < 5; i++) {
		if (key == pKeys[i]) { //valid input for player
			makeMove((GameConfig::MOVES)i);
		}
	}
	//every 15 ticks go down
	if ((elapsedTicks % 15 == 0) && elapsedTicks != 0) {
		moveDown();
		elapsedTicks = 0;
	}

}

//Computer class
//--------------
void Computer::checkInput(const char& key) { //check computer player's route 

	elapsedTicks++;
	
	//simulate key presses for computer
	//check if route has any needed block moves to succeed
	if (!currentMove.empty()) {
		GameConfig::MOVES move = currentMove.front().second;
		if (currentMove.front().second != GameConfig::MOVES::DOWN && currentMove.front().second != GameConfig::MOVES::NONE) {
			makeMove(move);
			if (move != GameConfig::MOVES::DROP)
				currentMove.erase(currentMove.begin());
		}
		else if ((elapsedTicks % 15 == 0) && elapsedTicks != 0) {
			moveDown();
			if (move == GameConfig::MOVES::DOWN) //for adjusting current route to match block and game state
				currentMove.erase(currentMove.begin());
			elapsedTicks = 0;
		}
	}

}

void Computer::calculateMoves(Board& board, Tetromino& cBlock) { //get possible routes for current block for computer 


	//check if block has space to spawn
	//if different implementation potentially won't need
	if (!cBlock.validMove(board, 0, 0, pDrawOffset))
		return;

	possibleRoutes.clear();
	currentMove.clear();

	vector<pair<Point, GameConfig::MOVES>> route;

	findDown(cBlock, board, route);
	recursiveFindLeft(cBlock, board, route);
	recursiveFindRight(cBlock, board, route);

	if (possibleRoutes.empty()) //no route found
		return;

	//choose move based on difficulty
	//first move is the 'best' move
	//second is the 'bad' move
	if (difficulty == GameConfig::DIFFICULTY::BEST)
		currentMove.swap(possibleRoutes.front().second);
	else if (difficulty == GameConfig::DIFFICULTY::GOOD) {
		if (rand() % 40 == 1) //2.5% chance of choosing an average move
			currentMove.swap(possibleRoutes.back().second);
		else
			currentMove.swap(possibleRoutes.front().second);
	}
	else {
		if (rand() % 10 == 1) //10% chance of choosing an average move
			currentMove.swap(possibleRoutes.back().second);
		else
			currentMove.swap(possibleRoutes.front().second);
	}

}


void Computer::findDown(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route) { //move block in all rotation states to bottom

	for (int i = 0; i < 4; ++i) {
		goDownAndAdd(block, board, route);
		if (!block.rotatable())
			break;
		block.rotateBlock(GameConfig::ROTATION_DIR::CW);
		if (block.validMove(board, 0, 0, pDrawOffset)) {
			block.moveBlock(0, 0, false);
			route.push_back({ block.getOriginPoint(), GameConfig::MOVES::RCW });
		}
		else
			break;
	}
}

void Computer::recursiveFindLeft(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route) { //recursively move block left and then go down

	int rotState = block.getRotationState();

	//if valid, move and add pos and move type to route
	if (block.validMove(board, -1, 0, pDrawOffset)) {
		route.push_back({ block.getOriginPoint(), GameConfig::MOVES::LEFT });
		block.moveBlock(-1, 0, false);
		recursiveFindLeft(block, board, route);
	}
	else { //invalid move, rotate and move left
		for (int i = 0; i < 3 - rotState; ++i) {
			if (!rotateAndFind(block, board, route))
				return;
			recursiveFindLeft(block, board, route);
		}
		return;
	}
	goDownAndAdd(block, board, route); //finished a valid move
	for (int i = 0; i < 3 - rotState; ++i) {
		if (!rotateAndFind(block, board, route))
			break;
	}

}

void Computer::recursiveFindRight(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route) { //recursively move block right and then go down

	int rotState = block.getRotationState();

	//if valid, move and add node to route
	if (block.validMove(board, 1, 0, pDrawOffset)) {
		route.push_back({ block.getOriginPoint(), GameConfig::MOVES::RIGHT });
		block.moveBlock(1, 0, false);
		recursiveFindRight(block, board, route);
	}
	else {
		for (int i = 0; i < 3 - rotState; ++i) { //invalid move, rotate and move right
			if (!rotateAndFind(block, board, route))
				return;
			recursiveFindRight(block, board, route);
		}
		return;
	}
	goDownAndAdd(block, board, route);
	for (int i = 0; i < 3 - rotState; ++i) { //finished a valid move
		if (!rotateAndFind(block, board, route))
			break;
	}

}

bool Computer::rotateAndFind(Tetromino& block, const Board& board, vector<pair<Point, GameConfig::MOVES>>& route) { //rotate block and move it down to bottom

	if (!block.rotatable())
		return false;

	//if valid rotation rotate block and add node to route
	block.rotateBlock(GameConfig::ROTATION_DIR::CW);
	if (block.validMove(board, 0, 0, pDrawOffset)) {
		block.moveBlock(0, 0, false);
		route.push_back({ block.getOriginPoint(), GameConfig::MOVES::RCW });
		goDownAndAdd(block, board, route);
		return true;
	}

	return false;

}

void Computer::goDownAndAdd(Tetromino block, const Board& board, vector<pair<Point, GameConfig::MOVES>> route) { //move block down until limit reached

	bool drop = false;

	if (rand() % 5 == 1) //20% to initiate a 'drop' move
		drop = true;
	if (block.validMove(board, 0, 1, pDrawOffset)) {
		if (drop)
			route.push_back({ block.getOriginPoint(), GameConfig::MOVES::DROP });
		else {
			route.push_back({ block.getOriginPoint(), GameConfig::MOVES::DOWN });
			block.moveBlock(0, 1, false);
		}

	}

	while (block.validMove(board, 0, 1, pDrawOffset)) {
		if (!drop)
			route.push_back({ block.getOriginPoint(), GameConfig::MOVES::DOWN });
		block.moveBlock(0, 1, false);
	}

	float score = evaluateRoute(block, board);
	addRoute(score, block.getOriginPoint(), route);

}

void Computer::addRoute(const float& routeScore, const Point& pos, vector<pair<Point, GameConfig::MOVES>>& route) { //add route to possible routes

	route.push_back({ pos, GameConfig::MOVES::NONE });	//signifies end of route

	//sort initial two starting moves
	if (!possibleRoutes.empty() && possibleRoutes.size() == 1) {
		if (routeScore > possibleRoutes.front().first) //add to beginning
			possibleRoutes.insert(possibleRoutes.begin(), { routeScore, route });
		else //add to end
			possibleRoutes.push_back({ routeScore, route });
		return;
	}

	//replace route with one of the current possible routes if applicable
	if (!possibleRoutes.empty() && possibleRoutes.size() == 2) {
		if (routeScore > possibleRoutes.front().first) { //replace first element
			possibleRoutes.erase(possibleRoutes.begin());
			possibleRoutes.insert(possibleRoutes.begin(), { routeScore, route });
		}
		else if (routeScore < possibleRoutes.back().first) { //replace last (second) element
			possibleRoutes.pop_back();
			possibleRoutes.push_back({ routeScore, route });
		}
		return;
	}

	//add normally
	possibleRoutes.push_back({ routeScore, route });

}

float Computer::evaluateRoute(const Tetromino& currentBlock, Board board) const { //evaluate and grade possible route

	int rCompleted, holes, sumColHeights, bumpiness, highestPoint;
	float totalScore = 0.0f;

	if (currentBlock.getBlockID() == (int)GameConfig::BLOCK_IDS::bombBlock)
		board.explodeBomb(currentBlock.getShapePointPosY(0), currentBlock.getShapePointPosX(0) - pDrawOffset, pDrawOffset, false);
	else {
		for (int i = 0; i < 4; i++)
			board.setNonEmptySpace(currentBlock.getShapePointPosY(i), currentBlock.getShapePointPosX(i) - Player::getDrawOffset(), currentBlock.getBlockID());
	}

	highestPoint = board.getHighestPoint();
	bumpiness = board.calcBumpiness();
	sumColHeights = board.calcColHeights();
	holes = board.countHolesInBoard();
	rCompleted = board.checkCompletedRows();

	totalScore = (8.92037f * rCompleted) - (10.0f * holes) - (3.92605f * bumpiness) - (10.0f * sumColHeights) - (0.650103f * highestPoint);

	return totalScore;

}