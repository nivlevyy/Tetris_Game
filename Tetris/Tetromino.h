#pragma once

#include "Board.h"

class Tetromino {

	int blockID;
	int rotState; //0-3 for four rotation states
	int prevRotState;
	bool rotated;
	Point originPoint; //ref point for drawing blocks in middle of board
	Point prevOriginPoint;
	std::vector<Point> currShapeBody;
	std::vector<Point> prevShapeBody;

protected:

	void copyShape(const std::vector<Point>& tBody, const int& id);

private:

	void undoRotation();

public:

	Tetromino();
	int getRotationState() const { return rotState; }
	int getBlockID() const { return blockID; }
	void eraseBlock() const;
	void drawBlock() const;
	void rotateBlock(GameConfig::ROTATION_DIR dir);
	void moveBlock(const int& xOffset, const int& yOffset, const bool& draw);
	int getShapePointPosX(int index) const;
	int getShapePointPosY(int index) const;
	Point getOriginPoint() const { return originPoint; }
	void setOriginPoints(int originX, int originY);
	bool validMove(const Board& board, const int& xOffset, const int& yOffset, const int& dOffset);
	bool rotatable();

};