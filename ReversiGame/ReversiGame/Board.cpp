/*
* File: Board.cpp
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	All the board related logic and connecting with the game logic is done here.
*/

#include <math.h>
#include "Board.h"

Board* Board::s_instance = NULL;

Board::Board()
{
	gridShift = 0.2f;
	gameLogic = new GameLogic();
}

//Only one instance of the Board object exists
Board* Board::Instance()
{
	if(!s_instance)
		s_instance = new Board;
	return s_instance;
}

Board::~Board()
{}

Board::Board(Board const&)
{}

void Board::operator=(Board const&)
{}

float Board::getGridShift()
{
	return gridShift;
}

bool Board::validateBounds(float x, float y)
{
	//Detect if the click coordinates is inside the game board.
	if(x >= -0.8f && x < 0.8f && y <= 0.8f && y > -0.8f)	//Temp hardcoded and will never change. If screen size changes opengl handles the calculation for us.
		return true;
	else
		return false;
}

void Board::addCoin(PLAYERTYPE playerType, float x, float y)
{
	bool existsOnBoard = false;

	//Validate the mouse click is inside the board.
	if(!validateBounds(x, y))
	{
		cout << "Cannot place a coin outside the board." << endl;
		return;
	}

	//Check if the cell on the board doesnt have a coin already on it.
	for(int i=0; i<(int)pieces.size(); i++)
	{
		if(x == pieces[i].posX && y == pieces[i].posY)
		{
			existsOnBoard = true;
			break;
		}
	}

	if(existsOnBoard)
		cout << "Invalid move, another coin already there." << endl;
	else
	{
		//This is done seperately to keep both files data independent as one 
		//	is related to graphics and one is logic.

		//Add the gamelogic coins(GameLogic.boardmap) array
		int array_x_loc = (int)(ceilf( ((0.8f + x)/gridShift) * 100)/100);
		int array_y_loc = (int)(ceilf( ((0.8f - y)/gridShift) * 100)/100);
		bool coinSuccessfullyAdded = gameLogic->ValidateAndAddCoin(playerType, array_x_loc, array_y_loc);

		//first 4 coins in othello are placed for us.
		if(coinSuccessfullyAdded && (gameLogic->coinCount >= 4))
		{
			pieces.clear();
			//Update the graphics array to match the logic array.(Done when flipping of coins occur)
			for(int i=0; i<8; i++)
			{
				for(int j=0; j<8; j++)
				{
					if(gameLogic->boardMap[i][j] != emptyCharInArray)
					{
						Piece coin;
						coin.posX = (0.2f * (i) - 0.8f);
						coin.posY = -(0.2f * (j) - 0.8f);

						if(gameLogic->boardMap[i][j] == 'B')
							coin.playerType = BLACKCOIN;
						else if(gameLogic->boardMap[i][j] == 'W')
							coin.playerType = WHITECOIN;

						pieces.push_back(coin);
				
						gameLogic->coinCount++;
					}
				}
			}

			//Flip the players turn
			blackCoinTurn = !blackCoinTurn;

			pair<int, int> scores = gameLogic->getPlayersScore();
			//Display the end game if the game ended
			if(gameLogic->isGameEnded())
			{
				if(scores.first > scores.second)
					cout << endl << playerOneName << " wins the game" << endl;
				else
					cout << endl << playerTwoName << " wins the game" << endl;
			}
			else
			{
				//Display the score
				cout << playerOneName << " coins: " << scores.first << "\t" << playerTwoName << " coins: " << scores.second << endl;
				displayPlayerTurn();
			}
		}
		//Add the 4 default coins.
		else if(gameLogic->coinCount < 4)
		{
			Piece coin;
			coin.posX = x;
			coin.posY = y;
			coin.playerType = playerType;
			pieces.push_back(coin);
			gameLogic->coinCount++;
		}
		else if(!coinSuccessfullyAdded)
		{
			cout << "Invalid Move" << endl;
		}
	}

	return;
}

//Converts the mouse click dimesions of the game to float based upon our game dimensions (-0.8, 0.8)
GameDimension Board::convertToGameDimension(double windowXCoordinate, double windowYCoordinate, int gameWidth, int gameHeight)
{
	GameDimension gd;
	gd.x = (((float)windowXCoordinate/gameWidth) * 2.0f) - 1.0f;
	gd.y = 1.0f - (((float)windowYCoordinate/gameHeight) * 2.0f);

	return gd;
}

void Board::createCoin(PLAYERTYPE playerType, GameDimension inCellDimension)
{
	pair<float, float> topLeftCellPoint = detectCellPointTopLeft(inCellDimension);
	Board::Instance()->addCoin(playerType, topLeftCellPoint.first, topLeftCellPoint.second);

	//For Testing graphics array with game logic array uncomment this line.
	//gameLogic->ShowBoard();
}

//Detects the Top Left corner of the cell grid to place the graphics texture.
pair<float, float> Board::detectCellPointTopLeft(GameDimension inCellDimesion)
{
	float x = 0.0f;
	float y = 0.0f;
	int numOfCells = 8;
	float diff_x = 0.0f;
	float diff_y = 0.0f;

	//Calculation of the Top Left dimension based upon our game.
	if(validateBounds(inCellDimesion.x, inCellDimesion.y))
	{	
		diff_x = inCellDimesion.x - gridShift;
		diff_y = inCellDimesion.y + gridShift;
		float cellTopLeft_x = 0.0f;
		float cellTopLeft_y = 0.0f;
		for(int i=0; i<numOfCells+1; i++)
		{
			cellTopLeft_x = (-0.8f + (gridShift * i));
			if(diff_x < cellTopLeft_x)
			{
				x =  cellTopLeft_x;
				break;
			}
		}

		for(int i=0; i<numOfCells; i++)
		{
			cellTopLeft_y = (0.8f - (gridShift * i));
			if(diff_y > cellTopLeft_y)
			{
				y = cellTopLeft_y;
				break;
			}
		}
	}
	else
	{
		x = -1.0f;  //-1 added to force out of board if clicked outside.
		y = -1.0f;  //-1 added to force out of board if clicked outside.
	}
	return pair<float, float>(x,y);
}

void Board::resetGame()
{
	//Clear the pieces
	pieces.clear();
	
	//Clear the boardmap
	gameLogic->resetBoard();
	
	//Add the 4 default coins
	addCoin(WHITECOIN, -gridShift, gridShift);
	addCoin(BLACKCOIN, 0.0f, gridShift);
	addCoin(BLACKCOIN, -gridShift, 0.0f);
	addCoin(WHITECOIN, 0.0f, 0.0f);

	//Set the black coin player to go first.
	blackCoinTurn = true;
	displayPlayerTurn();
}

void Board::displayPlayerTurn()
{
	if(blackCoinTurn)
		cout << playerOneName << " your turn" << endl;
	else
		cout << playerTwoName << " your turn" << endl;
}

void Board::createPlayers()
{
	cout << "Welcome to Reversi" << endl;
	cout << "------------------" << endl;
	cout << "Enter player's name: ";
	cin >> playerOneName;
	cout << "Enter opponent's name: ";
	cin >> playerTwoName;

	cout << endl << "Hello " << playerOneName << " you are black coin" << endl;
	cout << "Hello " << playerTwoName << " you are white coin" << endl << endl;
}