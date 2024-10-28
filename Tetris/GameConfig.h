#pragma once

#include "GeneralFunctions.h"

class Tetromino; //FD for tetrominoes array

class GameConfig {

public:

	static bool useColors;
	static constexpr int BOARD_WIDTH = 12;
	static constexpr int BOARD_HEIGHT = 18;
	static constexpr int defBlockXOrigin = 4; //default block X origin point
	static constexpr int defBlockYOrigin = 1; //default block Y origin point
	static constexpr int gameTiming = 750; //750 ms delay between moveDown's
	static constexpr char blockSymbol = 'x';
	static constexpr char emptySpace = '0';
	static constexpr char ESC = 27;
	static const int COLORS[];
	static const Tetromino tetrominoes[]; //pool of the 7 types of blocks
	enum PLAYERS_ID { player1 = 0, player2 = 1 };
	enum CONTROLS_P1 { LEFT_a = 'a', RIGHT_d = 'd', ROTATE_CW_s = 's', ROTATE_CCW_w = 'w', DROP_x = 'x' };
	enum CONTROLS_P2 { LEFT_j = 'j', RIGHT_l = 'l', ROTATE_CW_k = 'k', ROTATE_CCW_i = 'i', DROP_m = 'm' };
	enum class MENU_MP_OPTIONS { START_HVSH = '1', START_HVSC = '2', START_CVSC = '3', RESUME_GAME = '4' };
	enum class MENU_SP_OPTIONS { START_GAME = '1', RESUME_GAME = '2' };
	enum class MENU_GENERAL_OPTIONS { TOGGLE_2PLAYERS = '6', TOGGLE_COLORS = '7', INSTRUCTIONS_CONTROLS = '8', EXIT = '9' };
	enum class MOVES { LEFT, RIGHT, DROP, RCW, RCCW, DOWN, NONE };
	enum class BLOCK_IDS { IBlock = 1, TBlock, JBlock, OBlock, LBlock, SBlock, ZBlock, bombBlock };
	enum class ROTATION_DIR { CW, CCW };
	enum class MODES { HumanVSNothing = 0, HumanVSHuman = '1', HumanVSComputer = '2', ComputerVSComputer = '3' };
	enum PLAYER_TYPES { HUMAN, COMPUTER };
	enum class DIFFICULTY { BEST = 'a', GOOD = 'b', NOVICE = 'c', NONE = 0 };
};