/*
* File: GameLogic.h
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	This is the game logic of the game. Kept it seperate as it is a requirement.
*/

#ifndef GAMELOGIC_HEADER
#define GAMELOGIC_HEADER

#include "main.h"

const int numberOfDirections = 8;	//Used in game logic to identify if all directions failed for placing a coin.
const char emptyCharInArray = '-';	//Used in game logic array for empty(no coins) locations.

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	char boardMap[8][8];	//Array to map the location of each coins.

	void resetBoard();
	void ShowBoard();
	bool ValidateAndAddCoin(PLAYERTYPE playerType, int moveToRow, int moveToCol);
	char getPlayerChar(PLAYERTYPE playerType);
	int coinCount;			//Total number of coins placed.
	int boardSize;
	void switchPlayerDisplay();
	bool isGameEnded();
	pair<int, int> getPlayersScore();

private:
	bool ValidateAndFlipCoins(PLAYERTYPE playerType, int moveToRowPlaced, int moveToColPlaced);
};

#endif