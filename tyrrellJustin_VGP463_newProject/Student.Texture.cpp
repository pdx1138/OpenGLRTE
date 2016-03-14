// Student.Texture.cpp
///////////////////////////////////////////////////////////////////////////////////////////////
#include "Student.Texture.h"
#include "CORE.File.Bmp.h"

Texture::Texture() {
	this->pFilename = NULL;
	this->nWidth = 0;
	this->nHeight = 0;
	this->id = 0;
	this->data = NULL;
	this->nActualWidth = 0;
}

Texture* Texture::LoadTexture(char* filename) {//, int width, int height) {
	BMP_FILE bmp;
	if(bmp.Load(filename) == false) return nullptr;

	// If width and height are undefined, set them according to the image size ??
	this->nWidth = bmp.Width();
	this->nHeight = bmp.Height();
	this->pFilename = filename;
	this->data = (int*)bmp.m_pRGB;
	this->id = -1;
}

void Texture::UnloadTexture() {
	delete [] this->data;
	this->pFilename = NULL;
	this->nWidth = 0;
	this->nHeight = 0;
	this->id = 0;
	this->nActualWidth = 0;
}
