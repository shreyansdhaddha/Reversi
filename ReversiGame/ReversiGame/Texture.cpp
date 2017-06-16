/*
* File: Texture.cpp
* Author: Shreyans Dhaddha
* Date: March 17, 2015
*	Texture class to create a texture from TGA file only.
*/
#include "Texture.h"
#include <fstream>

Texture::Texture(string filename)
{
	//Extra the information from the TGA file
	Header_TGA textureHeader;
	ifstream file(filename, ios_base::binary);
	
	if(!file.is_open())
		cout << "File cannot be opened" << endl; // Should change to an error callback instead

	//if(file == NULL)
		//cout << "File cannot be opened" << endl; // Should change to an error callback instead

	if(!file.read((char*) &textureHeader, sizeof(textureHeader)))
		cout << "File header cannot be read" << endl; // Should change to an error callback instead

	if(textureHeader.pixelDepth != 24 && textureHeader.pixelDepth != 32)
		cout << "File has incorrect bits" << endl; // Should change to an error callback instead

	if(textureHeader.width <= 0 || textureHeader.height <= 0)
		cout << "File is corrupted. Incorrect dimensions" << endl; // Should change to an error callback instead

	GLuint type = GL_RGBA;
	if(textureHeader.pixelDepth == 24)
		type = GL_RGB;

	GLuint bytesPerPixel= textureHeader.pixelDepth/8;
	GLuint imageSize = textureHeader.width * textureHeader.height * bytesPerPixel;
	imageData = new GLubyte[imageSize];
	
	if(!file.read((char*) imageData, imageSize))
		cout << "Cannot read image" << endl; // Should change to an error callback instead

	if(imageData == NULL)
		cout << "No image data" << endl; // Should change to an error callback instead

	//Flip BGR to RGB because TGA data is stored in BGR and we need RGB
	for(GLuint index = 0; index < (int)imageSize; index += bytesPerPixel )
	{
		GLuint swapColor = imageData[index];
		imageData[index] = imageData[index+2];
		imageData[index+2] = swapColor;
	}

	//Build the texuture
	if(!buildTexture(imageData, textureHeader.width, textureHeader.height, type))
		cout << "Texture build failed" << endl;
}

Texture::~Texture()
{
	delete [] imageData;
}

bool Texture::buildTexture(unsigned char* imageData, int width, int height, GLuint type)
{
	//Attach the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Define the specs of the image
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
	return true;
}

//Not Working
void Texture::buildFontTexture()
{
	fontBase = glGenLists(256);
	
	//Get Font letters
	for(int i =0; i < 256; i++)
	{
		float x = (float) (i % 16) / 16.0f;
		float y = (float) (i / 16) / 16.0f;

		glNewList(fontBase + i, GL_COMPILE);

		glBegin(GL_QUADS);
			glTexCoord2f(x, 1-y-0.625f); glVertex2i(0, 13);
			glTexCoord2f(x+0.625f, 1-y-0.625f); glVertex2i(13, 13);
			glTexCoord2f(x+0.625f, 1-y); glVertex2i(13, 0);
			glTexCoord2f(x, 1-y); glVertex2i(0, 0);
		glEnd();

		glTranslated(4, 0, 0);

		glEndList();
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
}