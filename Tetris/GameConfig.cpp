#include "GameConfig.h"
#include "Blocks.h"

const Tetromino GameConfig::tetrominoes[] = { IBlock(), TBlock(), JBlock(), OBlock(), LBlock(), SBlock(), ZBlock(), bombBlock() };
const int GameConfig::COLORS[] = { 191, 95, 31, 143, 111, 175 ,79, 64}; //background and foreground colors for tetrominoes
bool GameConfig::useColors = true;

