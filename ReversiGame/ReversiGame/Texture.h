/*
* File: Texture.h
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*/

#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include "main.h"

//TGA file's specifications in the header.
struct Header_TGA
{
	GLubyte id;
	GLubyte colorMapType;
	GLubyte imageType;
	GLubyte colorMapSpecification[5];
	GLushort xOrigin;
	GLushort yOrigin;
	GLushort width;
	GLushort height;
	GLubyte pixelDepth;
};

class Texture
{
public:
	//Variable
	unsigned char *imageData;
	GLuint textureID;
	GLuint fontBase;

	//Functions
	Texture(string filename);
	~Texture();
private:
	bool Texture::buildTexture(unsigned char* imageData, int width, int height, GLuint type);
	void Texture::buildFontTexture();			//Not used because not working.
};

#endif