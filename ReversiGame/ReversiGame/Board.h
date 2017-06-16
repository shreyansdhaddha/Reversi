/*
* File: Board.h
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	Only board related functions are here. This connects the Gamelogic with UI logic
*/

#ifndef BOARD_HEADER
#define BOARD_HEADER

#include "main.h"
#include <vector>
#include <algorithm>
#include "GameLogic.h"

//Structure for a coin
struct Piece
{
	PLAYERTYPE playerType;
	float posX;
	float posY;
};
//Dimension based on the screen width in float between 0 and 1.
struct GameDimension
{
	float x;
	float y;
	GameDimension()
	{
		x = 0.0f;
		y = 0.0f;
	}
};
class Board 
{
public:
	//Singleton Stuff
	static Board* Instance();		//Singleton since there can be only 1 board.
	
	//Variables
	vector<Piece> pieces;
	bool blackCoinTurn;			//Added since black coin player always moves first.
	char playerOneName[256];
	char playerTwoName[256];

	//Functions
	void addCoin(PLAYERTYPE playerType, float x, float y);
	GameDimension convertToGameDimension(double windowXCoordinate, double windowYCoordinate, int gameWidth, int gameHeight);
	void createCoin(PLAYERTYPE playerType, GameDimension inCellDimension);
	void resetGame();
	float getGridShift();
	void displayPlayerTurn();
	void createPlayers();
	void displayPlayerScores();

private:
	//Singleton Stuff
	static Board* s_instance;
	Board();
	~Board();
	Board(Board const&);			//Added to avoid allowing to create copy of object.
	void operator=(Board const&);	//Added to avoid allowing to create copy of object.
	
	//Variables
	float gridShift;		//Defines the shift of each cell in float between 0 and 1
	GameLogic* gameLogic;	//Game logic is sepearted based on requirements.

	//Functions
	bool validateBounds(float x, float y); // Validates the board bounds
	pair<float, float> detectCellPointTopLeft(GameDimension inCellDimesion); //Detects the mouse click and calculates the top left x and y coordinates for the texture is place.
};

#endif