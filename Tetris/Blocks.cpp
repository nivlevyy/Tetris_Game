#include "Blocks.h"

IBlock::IBlock() { //set IBlock default state

	body.emplace_back(0, 1);
	body.emplace_back(1, 1);
	body.emplace_back(2, 1);
	body.emplace_back(3, 1);

	copyShape(body, id);

}

TBlock::TBlock() { //set IBlock default state

	body.emplace_back(1, 0);
	body.emplace_back(0, 1);
	body.emplace_back(1, 1);
	body.emplace_back(2, 1);

	copyShape(body, id);

}

OBlock::OBlock() { //set IBlock default state

	body.emplace_back(1, 0);
	body.emplace_back(2, 0);
	body.emplace_back(1, 1);
	body.emplace_back(2, 1);

	copyShape(body, id);

}

JBlock::JBlock() { //set IBlock default state

	body.emplace_back(1, 0);
	body.emplace_back(1, 1);
	body.emplace_back(1, 2);
	body.emplace_back(0, 2);

	copyShape(body, id);

}

LBlock::LBlock() { //set IBlock default state

	body.emplace_back(1, 0);
	body.emplace_back(1, 1);
	body.emplace_back(1, 2);
	body.emplace_back(2, 2);

	copyShape(body, id);

}

SBlock::SBlock() { //set IBlock default state

	body.emplace_back(1, 0);
	body.emplace_back(2, 0);
	body.emplace_back(0, 1);
	body.emplace_back(1, 1);

	copyShape(body, id);

}

ZBlock::ZBlock() { //set IBlock default state

	body.emplace_back(0, 0);
	body.emplace_back(1, 0);
	body.emplace_back(1, 1);
	body.emplace_back(2, 1);

	copyShape(body, id);

}

bombBlock::bombBlock() {

	body.emplace_back(1, 0);

	copyShape(body, id);

}

