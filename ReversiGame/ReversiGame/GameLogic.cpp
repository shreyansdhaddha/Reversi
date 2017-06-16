/*
* File: GameLogic.cpp
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	All the backend game logic.
*/
#include "GameLogic.h"

GameLogic::GameLogic()
{
	coinCount = 0;
	boardSize = 8;
	resetBoard();
}

GameLogic::~GameLogic()
{

}

void GameLogic::resetBoard()
{
	for(int i=0; i < boardSize; i++)
	{
		for(int j=0; j < boardSize; j++)
		{
			boardMap[i][j] = emptyCharInArray;
		}
	}
}

//For testing purposes.
void GameLogic::ShowBoard()
{
	cout << "Display board for testing" << endl;
	cout << "##########" << endl;
	for(int i=0; i < 8; i++)
	{
		cout << "#";
		for(int j=0; j<8; j++)
		{
			cout << boardMap[j][i];
		}
		cout << "#" << endl;
	}
	cout << "##########" << endl;
}

bool GameLogic::ValidateAndAddCoin(PLAYERTYPE playerType, int moveToRow, int moveToCol)
{
	//Check if position is filled.
	if(boardMap[moveToRow][moveToCol] != emptyCharInArray)
		return false;

	//Allow first 4 coins.
	if(coinCount < 4)
	{
		boardMap[moveToRow][moveToCol] = getPlayerChar(playerType);
		return true;
	}

	//Temp Array to check bounds and check adj cells
	// Ex:		-	-	-
	//			-	(W)	B
	//			-	B	B
	// When (W) is added its going to check all the connecting cells to identify validity.
	int startingX = moveToRow-1;
	int startingY = moveToCol-1;
	bool coinPosValid = false;
	for(int i=startingX; i<startingX+3; i++)
	{
		for(int j=startingY; j<startingY+3; j++)
		{
			//Check all the other tiles
			char currVal = boardMap[i][j];
			if(currVal == 'B' || currVal == 'W')
			{
				coinPosValid = true;
				break;
			}
		}
		if(coinPosValid)
			break;
	}

	if(coinPosValid == true && ValidateAndFlipCoins(playerType, moveToRow, moveToCol))
		return true;

	return false;
}

char GameLogic::getPlayerChar(PLAYERTYPE playerType)
{
	if(playerType == BLACKCOIN)
		return 'B';
	else
		return 'W';
}

//Validate and Flips the coins as required.
bool GameLogic::ValidateAndFlipCoins(PLAYERTYPE playerType, int moveToRowPlaced, int moveToColPlaced)
{
	bool foundCoin = false;
	int notFoundInPlaces = 0;
	char playerChar = getPlayerChar(playerType);

	//Determines if the flip in a specific direction was found.
	bool flipCheckUp = false;
	bool flipCheckDown = false;
	bool flipCheckLeft = false;
	bool flipCheckRight = false;
	bool flipCheckToTopLeft = false;
	bool flipCheckToTopRight = false;
	bool flipCheckToBottomLeft = false;
	bool flipCheckToBottomRight = false;

	//Determines the flip index from the moveToRowPlaced and moveToColPlaced locations of a coin.
	//Used when a flip is occured and stores in the index location of the last coin to indicate all the coins
	//between the moveToRowPlaced or moveToColPlaced to the flip index variables are required to 
	//be flipped(change from black to white and vise-versa.
	int flipCheckUpIndex = 0;
	int flipCheckDownIndex = 0;
	int flipCheckLeftIndex = 0;
	int flipCheckRightIndex = 0;
	int flipCheckToTopLeftIndex = 0;
	int flipCheckToTopRightIndex = 0;
	int flipCheckToBottomLeftIndex = 0;
	int flipCheckToBottomRightIndex = 0;

	//Check Up (From the click location to Up to find a similar coin)
	flipCheckUp = false;
	for(int j=moveToColPlaced-1; j>=0; j--)
	{
		//If a similar coin is found then stop.
		if(boardMap[moveToRowPlaced][j] == playerChar && flipCheckUpIndex > 0)
		{
			flipCheckUp = true;
			break;
		}
		//Ignore the check if the loc is empty of out of bounds
		else if (boardMap[moveToRowPlaced][j] == emptyCharInArray || j == 0)
		{
			notFoundInPlaces++;
			break;
		}
		//Stores the last location of the coin in the search.
		flipCheckUpIndex = j;
	}

	//Check Down (From the click location to Down to find a similar coin)
	flipCheckDown = false;
	for(int j=moveToColPlaced+1; j<boardSize; j++)
	{
		if(boardMap[moveToRowPlaced][j] == playerChar && flipCheckDownIndex > 0)
		{
			flipCheckDown = true;
			break;
		}
		else if (boardMap[moveToRowPlaced][j] == emptyCharInArray || j == boardSize-1)
		{	
			notFoundInPlaces++;
			break;
		}
		flipCheckDownIndex = j;
	}


	//Check Left (From the click location to Left to find a similar coin)
	flipCheckLeft = false;
	for(int i=moveToRowPlaced-1; i>=0; i--)
	{
		if(boardMap[i][moveToColPlaced] == playerChar && flipCheckLeftIndex > 0)
		{
			flipCheckLeft = true;
			break;
		}
		else if (boardMap[i][moveToColPlaced] == emptyCharInArray || i == 0)
		{
			notFoundInPlaces++;
			break;
		}
		flipCheckLeftIndex = i;
	}

	//Check Right (From the click location to Right to find a similar coin)
	flipCheckRight = false;
	for(int i=moveToRowPlaced+1; i<boardSize; i++)
	{
		if(boardMap[i][moveToColPlaced] == playerChar && flipCheckRightIndex > 0)
		{
			flipCheckRight = true;
			break;
		}
		else if (boardMap[i][moveToColPlaced] == emptyCharInArray || i == boardSize-1)
		{
			notFoundInPlaces++;
			break;
		}
		flipCheckRightIndex = i;
	}

	//Check Diagonal Going To Top Left (From the click location to diagonal going to top left to find a similar coin)
	flipCheckToTopLeft = false;
	for(int i=moveToRowPlaced-1, j=moveToColPlaced-1; i>=0; i--, j--)
	{
		if(boardMap[i][j] == playerChar && flipCheckToTopLeftIndex > 0)
		{
			flipCheckToTopLeft = true;
			break;
		}
		else if (boardMap[i][j] == '-' || i == 0 || j == 0)
		{
			notFoundInPlaces++;
			break;
		}
		flipCheckToTopLeftIndex = i;
	}

	//Check Diagonal Going To Top Right (From the click location to diagonal going to top right to find a similar coin)
	flipCheckToTopRight = false;
	for(int i=moveToRowPlaced+1, j=moveToColPlaced-1; i<boardSize; i++, j--)
	{
		if(boardMap[i][j] == playerChar && flipCheckToTopRightIndex > 0)
		{
			flipCheckToTopRight = true;
			break;
		}
		else if (boardMap[i][j] == emptyCharInArray || i == boardSize-1 || j == 0)
		{
			notFoundInPlaces++;
			break;
		}

		flipCheckToTopRightIndex = i;
	}

	//Check Diagonal Going To Down Right (From the click location to diagonal going to down right to find a similar coin)
	flipCheckToBottomRight = false;
	for(int i=moveToRowPlaced+1, j=moveToColPlaced+1; i<boardSize; i++, j++)
	{
		if(boardMap[i][j] == playerChar && flipCheckToBottomRightIndex > 0)
		{
			flipCheckToBottomRight = true;
			break;
		}
		else if (boardMap[i][j] == emptyCharInArray || i == boardSize-1 || j == boardSize-1)
		{
			notFoundInPlaces++;
			break;
		}

		flipCheckToBottomRightIndex = i;
	}

	//Check Diagonal Going To Down Left (From the click location to diagonal going to down left to find a similar coin)
	flipCheckToBottomLeft = false;
	for(int i=moveToRowPlaced-1, j=moveToColPlaced+1; i<boardSize; i--, j++)
	{
		if(boardMap[i][j] == playerChar && flipCheckToBottomLeftIndex > 0)
		{
			flipCheckToBottomLeft = true;
			break;
		}
		else if (boardMap[i][j] == emptyCharInArray || i == boardSize-1 || j == boardSize-1)
		{
			notFoundInPlaces++;
			break;
		}

		flipCheckToBottomLeftIndex = i;
	}

	//If none of them have a coin in the middle for this coin placed to the next similar coin then its invalid move.
	if(notFoundInPlaces == numberOfDirections)
		return false;

	//If the index for all variables defaulted to 0 then its invalid move.
	if(flipCheckUpIndex==0 && flipCheckDownIndex==0 && flipCheckLeftIndex==0 && flipCheckRightIndex==0 && flipCheckToTopLeftIndex==0 && flipCheckToTopRightIndex==0 && flipCheckToBottomRightIndex==0 && flipCheckToBottomLeftIndex==0)
		return false;

	//If found coin
	if(flipCheckUp)
	{
		for(int j=flipCheckUpIndex; j<=moveToColPlaced && flipCheckUpIndex!=0; j++)
			boardMap[moveToRowPlaced][j] = playerChar;
	}
	if(flipCheckDown)
	{
		for(int j=moveToColPlaced; j<=flipCheckDownIndex && flipCheckDownIndex!=0; j++)
			boardMap[moveToRowPlaced][j] = playerChar;
	}
	if(flipCheckLeft)
	{
		for(int i=flipCheckLeftIndex; i<=moveToRowPlaced && flipCheckLeftIndex!=0; i++)
			boardMap[i][moveToColPlaced] = playerChar;
	}
	if(flipCheckRight)
	{
		for(int i=moveToRowPlaced; i<=flipCheckRightIndex && flipCheckRightIndex!=0; i++)
			boardMap[i][moveToColPlaced] = playerChar;
	}
	if(flipCheckToTopLeft)
	{
		for(int i=moveToRowPlaced, j=moveToColPlaced; i>=flipCheckToTopLeftIndex && flipCheckToTopLeftIndex!=0; i--, j--)
			boardMap[i][j] = playerChar;
	}
	if(flipCheckToTopRight)
	{
		for(int i=moveToRowPlaced, j=moveToColPlaced; i<=flipCheckToTopRightIndex && flipCheckToTopRightIndex!=0; i++, j--)
			boardMap[i][j] = playerChar;
	}
	if(flipCheckToBottomRight)
	{
		for(int i=moveToRowPlaced, j=moveToColPlaced; i<=flipCheckToBottomRightIndex && flipCheckToBottomRightIndex!=0; i++, j++)
			boardMap[i][j] = playerChar;
	}
	if(flipCheckToBottomLeft)
	{
		for(int i=moveToRowPlaced, j=moveToColPlaced; i>=flipCheckToBottomLeftIndex && flipCheckToBottomLeftIndex!=0; i--, j++)
			boardMap[i][j] = playerChar;
	}

	return true;
}

bool GameLogic::isGameEnded()
{
	for(int i=0; i<boardSize; i++)
	{
		for(int j=0; j<boardSize; j++)
		{
			if(boardMap[i][j] == emptyCharInArray)
				return false;
		}
	}

	return true;
}

pair<int, int> GameLogic::getPlayersScore()
{
	int player1Score = 0;
	int player2Score = 0;
	for(int i=0; i<boardSize; i++)
	{
		for(int j=0; j<boardSize; j++)
		{
			if(boardMap[i][j] == 'B')
				player1Score++;
			else if (boardMap[i][j] == 'W')
				player2Score++;
		}
	}
	return pair<int, int>(player1Score, player2Score);
}
