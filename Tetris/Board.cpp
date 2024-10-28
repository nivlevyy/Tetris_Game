#include "Board.h"

void Board::setNewBoard() { //set default board

	int i, j;

	//set empty spaces
	for (i = 1; i < GameConfig::BOARD_HEIGHT - 1; i++) {
		for (j = 1; j < GameConfig::BOARD_WIDTH - 1; j++) {
			this->grid[i][j] = GameConfig::emptySpace;
		}
	}
	//set outline of board
	for (i = 0; i < GameConfig::BOARD_HEIGHT; i++) {
		for (j = 0; j < GameConfig::BOARD_WIDTH; j++) {
			if (i == 0 || i == GameConfig::BOARD_HEIGHT - 1)
				this->grid[i][j] = '*';
			else if (j == 0 || j == GameConfig::BOARD_WIDTH - 1)
				this->grid[i][j] = '|';
		}
	}

}

void Board::printBoard(const int& dOffset) const { //print a player's board

	int i, j;

	gotoxy(0, 0);
	for (i = 0; i < GameConfig::BOARD_HEIGHT; i++) {
		for (j = 0; j < GameConfig::BOARD_WIDTH; j++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //set back to default color
			gotoxy(dOffset + j, i); //account for drawing offset
			if (grid[i][j] != '*' && grid[i][j] != '|' && grid[i][j] != GameConfig::emptySpace) { //block char
				if (GameConfig::useColors) //set color accordingly
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameConfig::COLORS[grid[i][j] - '0' - 1]);
				std::cout << GameConfig::blockSymbol;
			}
			else { //board outline or empty space
				grid[i][j] == GameConfig::emptySpace ? std::cout << ' ' : std::cout << grid[i][j];
			}
		}
		std::cout << std::endl;
	}

}

void Board::setNonEmptySpace(const int& row, const int& column, const int& blockID) { //set board cell to the given blockID

	grid[row][column] = blockID + '0';

}

bool Board::checkEmptySpace(const int& row, const int& column) const { //check if cell is empty 

	return grid[row][column] == GameConfig::emptySpace;

}

int Board::checkCompletedRows() { //check for completed rows in board

	int i, counter = 0;

	//count completed rows (if any)
	for (i = GameConfig::BOARD_HEIGHT - 2; i > 0; i--) {
		if (checkFullRow(i)) {
			counter++;
			clearRow(i);
		}
		else if (counter > 0) //use counter as offset to shift rows down
			ShiftRow(i, counter);
	}

	return counter;

}

void Board::explodeBomb(const int& rowInd, const int& colInd, const int& dOffset, const bool& draw) {	//'explode' bomb in diamond shape pattern

	for (int i = 4; i >= 0; --i) {
		for (int j = 0; j <= 4 - i; ++j) {
			if (rowInd + i < GameConfig::BOARD_HEIGHT - 1) {
				if (colInd + j < GameConfig::BOARD_WIDTH - 1) {
					grid[rowInd + i][colInd + j] = GameConfig::emptySpace;
					if (draw) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						gotoxy(colInd + j + dOffset, rowInd + i);
						std::cout << 'x';
					}
				}
				if (colInd - j > 0) {
					grid[rowInd + i][colInd - j] = GameConfig::emptySpace;
					if (draw) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						gotoxy(colInd - j + dOffset, rowInd + i);
						std::cout << 'x';
					}
				}

			}
			if (rowInd - i > 0) {
				if (colInd + j < GameConfig::BOARD_WIDTH - 1) {
					grid[rowInd - i][colInd + j] = GameConfig::emptySpace;
					if (draw) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						gotoxy(colInd + j + dOffset, rowInd - i);
						std::cout << 'x';
					}
				}
				if (colInd - j > 0) {
					grid[rowInd - i][colInd - j] = GameConfig::emptySpace;
					if (draw) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						gotoxy(colInd - j + dOffset, rowInd - i);
						std::cout << 'x';
					}
				}
			}
		}
	}
	if (draw)
		Sleep(25);

	shockBoard(rowInd + 4, colInd + 4);

}

void Board::shockBoard(const int& rowInd, const int& colInd) { //drop any blocks in bomb radius down

	int lEdge, rEdge;

	rEdge = colInd;
	lEdge = colInd - (2 * 4);

	//adjust shock radius edges if over limits
	if (rEdge > GameConfig::BOARD_WIDTH - 2)
		rEdge -= (rEdge - (GameConfig::BOARD_WIDTH - 2));
	if (lEdge < 1)
		lEdge += (1 - lEdge);

	if (lEdge > GameConfig::BOARD_WIDTH - 2 || rEdge < 1) //off limits
		return;

	for (int j = lEdge; j <= rEdge; ++j) {
		for (int i = GameConfig::BOARD_HEIGHT - 2; i > 0; --i) {
			if (!checkEmptySpace(i, j)) {
				int ind = i;
				while (checkEmptySpace(ind + 1, j)) {
					if (ind < 1 || ind > 16) //off limits
						break;
					grid[ind + 1][j] = grid[ind][j];
					grid[ind][j] = GameConfig::emptySpace;
					ind++;
				}
			}
		}
	}

}

bool Board::checkFullRow(const int& rowInd) const { //check if full row

	for (int j = 1; j < GameConfig::BOARD_WIDTH - 1; j++) {
		if (grid[rowInd][j] == GameConfig::emptySpace)
			return false;
	}

	return true;

}

void Board::clearRow(const int& rowInd) { //clear full row

	for (int j = 1; j < GameConfig::BOARD_WIDTH - 1; j++)
		grid[rowInd][j] = GameConfig::emptySpace;

}

void Board::ShiftRow(const int& rowInd, const int& rowOffset) { //updates board after clearing completed rows

	for (int j = 1; j < GameConfig::BOARD_WIDTH - 1; j++) {
		grid[rowInd + rowOffset][j] = grid[rowInd][j];
		grid[rowInd][j] = GameConfig::emptySpace;
	}

}

int Board::countHolesInBoard() const {

	int counter = 0;

	Board cpy;
	memcpy(cpy.grid, this->grid, sizeof(this->grid));

	for (int i = 2; i < GameConfig::BOARD_HEIGHT - 1; ++i) {
		for (int j = 1; j < GameConfig::BOARD_WIDTH - 1; ++j) {
			if (checkEmptySpace(i, j)) {
				if (!checkEmptySpace(i - 1, j) || cpy.grid[i - 1][j] == 'o') {
					cpy.grid[i][j] = 'o';
					counter++;
				}
			}
		}
	}

	return counter;

}

int Board::calcColHeights() const { //returns sum of column heights

	int res = 0;

	for (int j = 1; j < GameConfig::BOARD_WIDTH - 1; ++j) {
		for (int i = 1; i < GameConfig::BOARD_HEIGHT - 1; ++i) {
			if (!checkEmptySpace(i, j)) {
				res += (GameConfig::BOARD_HEIGHT - 1) - i;
				break;

			}
		}
	}

	return res;

}

int Board::calcBumpiness() const { //returns sum of absolute differences of adjacent columns

	int res, c1, c2;
	res = c1 = c2 = 0;

	for (int j = 1; j < GameConfig::BOARD_WIDTH - 2; ++j) {
		for (int i = 1; i < GameConfig::BOARD_HEIGHT - 1; ++i) {
			if (!checkEmptySpace(i, j)) {
				c1 = (GameConfig::BOARD_HEIGHT - 1) - i;
				break;
			}

		}
		for (int i = 1; i < GameConfig::BOARD_HEIGHT - 1; ++i) {
			if (!checkEmptySpace(i, j + 1)) {
				c2 = (GameConfig::BOARD_HEIGHT - 1) - i;
				break;
			}
		}
		res += abs(c1 - c2);
		c1 = c2 = 0;
	}

	return res;

}

int Board::getHighestPoint() const { //gets board's top point 

	int resY = 0;

	for (int i = 1; i < GameConfig::BOARD_HEIGHT - 1; ++i) {
		for (int j = 1; j < GameConfig::BOARD_WIDTH - 1; ++j) {
			if (!checkEmptySpace(i, j)) {
				resY = i;
				return (GameConfig::BOARD_HEIGHT - 1) - resY;
			}
		}
	}

	return resY;

}
