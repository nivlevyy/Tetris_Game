#include "Tetromino.h"

Tetromino::Tetromino() {

	prevRotState = rotState = 0;
	blockID = 0;
	rotated = false;

}

void Tetromino::eraseBlock() const { //erase block according to previous origin points

	if (rotated) { //delete pre-rotation points
		for (auto& itr : prevShapeBody)
			itr.erase(prevOriginPoint);
	}
	else { //directional move - no change to shape body
		for (auto& itr : currShapeBody)
			itr.erase(prevOriginPoint);
	}

}

void Tetromino::drawBlock() const { //draw block according to origin points

	for (auto& itr : currShapeBody)
		itr.draw(originPoint, blockID);

}

void Tetromino::rotateBlock(GameConfig::ROTATION_DIR dir) { //rotate block accordingly and update pos

	if (!rotatable())
		return;

	int maxHeight;

	rotated = true;
	prevRotState = rotState;
	copyShape(currShapeBody, blockID); //copies pre-rotation shape points

	if ((GameConfig::BLOCK_IDS)blockID == (GameConfig::BLOCK_IDS::IBlock)) 	//IBlock has different rotation point
		maxHeight = 4;
	else
		maxHeight = 3;

	//rotate in relation to maxHeight
	if (dir == GameConfig::ROTATION_DIR::CW) {
		rotState = (rotState + 1) % 4; //update rotation state
		for (auto& itr : currShapeBody)	 //assign new rotated points
			itr.setPoints(1 - (itr.getY() - (maxHeight - 2)), itr.getX());
	}
	else { //CCW
		rotState = ((rotState - 1) + 4) % 4; //update rotation state
		for (auto& itr : currShapeBody) //assign new rotated points
			itr.setPoints(itr.getY(), 1 - (itr.getX() - (maxHeight - 2)));
	}

}

void Tetromino::undoRotation() { //undo shape's rotation (if invalid)

	rotState = prevRotState;
	rotated = false;

	currShapeBody = prevShapeBody;	//copy back previous points

}

void Tetromino::moveBlock(const int& xOffset, const int& yOffset, const bool& draw) { //update block's new position and move it

	//for erasing
	prevOriginPoint.copyPoint(originPoint);

	//update new origin position according to offsets
	originPoint.setPoints(originPoint.getX() + xOffset, originPoint.getY() + yOffset);

	if (draw) {
		eraseBlock();
		drawBlock();
	}

	//finished move
	rotated = false;

}

void Tetromino::setOriginPoints(int originX, int originY) { //set given origin points for block drawing

	prevOriginPoint.copyPoint(originPoint);
	originPoint.setPoints(originX, originY); 

}

void Tetromino::copyShape(const std::vector<Point>& sBody, const int& id) { //copy shape's points

	if (rotated)
		prevShapeBody = sBody;
	else {
		currShapeBody = sBody;
		blockID = id;
	}

}

bool Tetromino::validMove(const Board& board, const int& xOffset, const int& yOffset, const int& dOffset) { //check if player move is valid

	int i, size = currShapeBody.size();
	bool valid = true;

	//check for collision
	//substract drawing offset from
	//position to match board array
	for (i = 0; i < size; i++) {
		if (!board.checkEmptySpace(getShapePointPosY(i) + yOffset, (getShapePointPosX(i) + xOffset) - dOffset)) {
			valid = false;
			break;
		}
	}

	if (rotated && !valid)
		undoRotation();

	return valid;

}

int Tetromino::getShapePointPosX(int index) const { //return X coord of shape point on screen

	return (currShapeBody.begin() + index)->getX() + originPoint.getX();

}

int Tetromino::getShapePointPosY(int index) const { //return Y coord of shape point on screen

	return (currShapeBody.begin() + index)->getY() + originPoint.getY();

}

bool Tetromino::rotatable() { //check if block is rotatable

	if (blockID == (int)GameConfig::BLOCK_IDS::OBlock)
		return false;
	else if (blockID == (int)GameConfig::BLOCK_IDS::bombBlock)
		return false;

	return true;

}
