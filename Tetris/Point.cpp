#include "Point.h"

void Point::draw(const Point& ref, int colorInd) const { //draw a point on screen (use color if needed)

	if (GameConfig::useColors) //set proper color if needed
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameConfig::COLORS[colorInd - 1]);
	else //set to default color
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	gotoxy(x + ref.x, y + ref.y);
	std::cout << GameConfig::blockSymbol;

}
void Point::erase(const Point& ref) const { //erase a point from screen

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(x + ref.x, y + ref.y);
	std::cout << ' ';

}