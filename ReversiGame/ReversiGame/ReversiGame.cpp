#define _USE_MATH_DEFINES
/*
* File: Reversi.cpp
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	All the OpenGl related function as well as some helper functions due to singleton being used.
*/

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include "Texture.h"
#include "GLFunctions.h"
using namespace std;

//Error callback for opengl related calls.
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

//Key callback for keyboard related keys.
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

//Not used. Was going to use this for detecting where the coins can be place. But skipped due to time constraints.
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
}

//Mouse button click callback.
static void mouse_button_press_callback(GLFWwindow* window, int key, int action, int mods){
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		GLFunctions::Instance()->createCoin(x, y);
	}
}

int main(void)
{
	//Setup the openGL window and functions
    GLFWwindow* window;
	GLFunctions::Instance()->init();
	int width = GLFunctions::Instance()->getWidth();
	int height = GLFunctions::Instance()->getHeight();

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
		exit(EXIT_FAILURE);
    window = glfwCreateWindow(width, height, "iReversi (Othello)", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

	//Set the callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_press_callback);
	//glfwSetCursorPosCallback(window, cursor_pos_callback);
	
	
	//Define the textures.
	Texture *bgTexture = new Texture("background.tga");
	Texture *blackCoinTexture = new Texture("blackcoin.tga");
	Texture *whiteCoinTexture = new Texture("whitecoin.tga");
	//Texture *fontTexture = new Texture("font.tga");
	
	//Setup and start the game.
	GLFunctions::Instance()->createPlayers();
	GLFunctions::Instance()->startGame();

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLFunctions::Instance()->createBackground(bgTexture->textureID);
		GLFunctions::Instance()->createGrid(width, height);
		GLFunctions::Instance()->updateCoinsOnView(blackCoinTexture->textureID, whiteCoinTexture->textureID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}