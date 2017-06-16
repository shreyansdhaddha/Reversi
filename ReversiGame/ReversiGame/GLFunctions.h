/*
* File: GLFunctions.h
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	Only has functions related to openGL.
*/

#ifndef GLFUNCTIONS_HEADER
#define GLFUNCTIONS_HEADER

#include "main.h"
#include "Texture.h"

class GLFunctions
{
public:
	//Singleton Stuff
	static GLFunctions* Instance();			//Singleton to restrict from creating a object of this class.

	//Variables
	int width;
	int height;

	//Functions
	void init();
	int getWidth();
	int getHeight();
	void startGame();
	void createCoin(double x, double y);
	void createGrid(int width, int height);												//Creates the game box grid.
	void createBackground(GLuint textureID);
	void updateCoinsOnView(GLuint blackCoinTextureID, GLuint whiteCoinTextureID);		//Updates all the coins every frame.
	void createPlayers();

private:
	//Singleton Stuff
	static GLFunctions* s_instance;
	GLFunctions();
	~GLFunctions();
	GLFunctions(GLFunctions const&);		//Added to avoid allowing to create copy of object.
	void operator=(GLFunctions const&);		//Added to avoid allowing to create copy of object.
};

#endif