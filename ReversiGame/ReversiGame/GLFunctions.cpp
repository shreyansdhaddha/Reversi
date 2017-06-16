/*
* File: GLFunction.cpp
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	All the OpenGl related function as well as some helper functions due to singleton being used.
*/
#include "GLFunctions.h"
#include "Board.h"
#include <string>

GLFunctions* GLFunctions::s_instance = NULL;

GLFunctions::GLFunctions()
{
}

GLFunctions* GLFunctions::Instance()
{
	if(!s_instance)
		s_instance = new GLFunctions;
	return s_instance;
}

GLFunctions::~GLFunctions()
{}

GLFunctions::GLFunctions(GLFunctions const&)
{}

void GLFunctions::operator=(GLFunctions const&)
{}

void GLFunctions::init()
{
	width = 800;
	height = 800;
}

int GLFunctions::getWidth()
{
	return width;
}

int GLFunctions::getHeight()
{
	return height;
}

void GLFunctions::createCoin(double x, double y)
{
	PLAYERTYPE playerType;
	GameDimension gd = Board::Instance()->convertToGameDimension(x, y, getWidth(), getHeight());
	if(Board::Instance()->blackCoinTurn)
		playerType = BLACKCOIN;
	else
		playerType = WHITECOIN;
	
	Board::Instance()->createCoin(playerType, gd);
}

void GLFunctions::createPlayers()
{
	Board::Instance()->createPlayers();
}

void GLFunctions::startGame()
{
	Board::Instance()->resetGame();
}

//Update the coins every frame per second.
void GLFunctions::updateCoinsOnView(GLuint blackCoinTextureID, GLuint whiteCoinTextureID)
{
	float x;
	float y;
	float gridShift = Board::Instance()->getGridShift();

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	int numberOfCoins = Board::Instance()->pieces.size();
	for(int i=0; i<numberOfCoins; i++)
	{
		x = Board::Instance()->pieces[i].posX;
		y = Board::Instance()->pieces[i].posY;

		if(Board::Instance()->pieces[i].playerType == BLACKCOIN)
		{
			glBindTexture(GL_TEXTURE_2D, blackCoinTextureID);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, whiteCoinTextureID);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x, y);							// Top Left
			glTexCoord2f(0.0f, 1.0f); glVertex2f( x, y - gridShift);			// Top Right
			glTexCoord2f(0.0f, 0.0f); glVertex2f( x + gridShift, y-gridShift);	//	Bottom Right
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x + gridShift, y);				// Bottom Left
		glEnd();
		glFlush();
	}
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void GLFunctions::createGrid(int width, int height)
{
	float fboxSize = 0.8f;

	glColor4f(0.545f, 0.271f, 0.075f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-0.8f,0.8f);
		glVertex2f(0.8f, 0.8f);
		glVertex2f(0.8f, -0.8f);
		glVertex2f(-0.8f, -0.8f);
    glEnd();
	glFlush();

	glBegin(GL_LINES);
		int numOfCol = 8;
		float posX = ((fboxSize / 8.0f)*2.0f) - fboxSize;
		float posY = fboxSize - ((fboxSize / 8.0f)*2.0f);
		for(int i=0; i < numOfCol-1; i++)
		{
			//Columns
			glColor4f(0.4f, 0.8f, 0.75f, 1.0f);
			glVertex2f(posX, fboxSize);
			glVertex2f(posX, -fboxSize);
			posX +=0.2f;
						
			//Rows
			glColor4f(0.2f, 0.9f, 0.4f, 1.0f);
			glVertex2f(-fboxSize, posY);
			glVertex2f(fboxSize, posY);
			posY -=0.2f;
		}

	glEnd();
	glFlush();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLFunctions::createBackground(GLuint textureID)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);              // Top Left
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, 1.0f);              // Top Right
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);              //	Bottom Right
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);              // Bottom Left
	glEnd();
	glFlush();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}