#pragma once

#include "Tetromino.h"

class IBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::IBlock;
	std::vector<Point> body;

public:

	IBlock();

};

class TBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::TBlock;
	std::vector<Point> body;

public:

	TBlock();
};

class JBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::JBlock;
	std::vector<Point> body;

public:

	JBlock();

};

class OBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::OBlock;
	std::vector<Point> body;

public:

	OBlock();
};

class LBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::LBlock;
	std::vector<Point> body;

public:

	LBlock();

};

class SBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::SBlock;
	std::vector<Point> body;

public:

	SBlock();

};

class ZBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::ZBlock;
	std::vector<Point> body;

public:

	ZBlock();

};

class bombBlock : public Tetromino {

	int id = (int)GameConfig::BLOCK_IDS::bombBlock;
	std::vector<Point> body;

public:

	bombBlock();

};