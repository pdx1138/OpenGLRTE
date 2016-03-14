// Student.Texture.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class Texture {
	public:
		int id;						// This texture's ID number
		int* data;					// This texture's color data
		int nWidth;					// This texture's width
		int nHeight;				// This texture's height
		int nActualWidth;			// ????????????????????????????????????????????
		
		char* pFilename;			// This texture's file location

		Texture();					// Default constructor function;

		Texture* LoadTexture(char* filename, int width, int height);
		Texture* LoadTexture(char* filename);
		void UnloadTexture();
};

