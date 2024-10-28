#pragma once

#include "Point.h"

class Board {

	char grid[GameConfig::BOARD_HEIGHT][GameConfig::BOARD_WIDTH];

public:

	void setNewBoard();
	void printBoard(const int& offset) const;
	void setNonEmptySpace(const int& row, const int& column, const int& blockID);
	bool checkEmptySpace(const int& row, const int& column) const;
	int checkCompletedRows();
	int countHolesInBoard() const;
	int calcColHeights() const;
	int calcBumpiness() const;
	int getHighestPoint() const;
	void explodeBomb(const int& rowInd, const int& colInd, const int& dOffset, const bool& draw);

private:

	void shockBoard(const int& rowInd, const int& colInd);
	void clearRow(const int& rowInd);
	bool checkFullRow(const int& rowInd) const;
	void ShiftRow(const int& rowInd, const int& rowOffset);

};