#pragma once

#include "GameConfig.h"

class Point {

	int x, y;

public:

	Point() { x = y = 0; };
	Point(int _x, int _y) : x(_x), y(_y) {}
	void draw(const Point& ref, int colorInd) const;
	void erase(const Point& ref) const;
	void setPoints(const int& _x, const int& _y) { x = _x;	y = _y; }
	void copyPoint(const Point& ref) { x = ref.x;	y = ref.y; }
	int getX() const { return x; }
	int getY() const { return y; }
	bool operator==(const Point& ref) const { return (ref.x == x && ref.y == y); }
};

