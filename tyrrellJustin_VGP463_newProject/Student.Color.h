// Student.Color.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Student.Texture.h"
#include "Student.Point.h"

namespace COLOR {

	struct Color {
		float r, g, b, a;

		Color();
		Color(float r, float g, float b, float a);
		Color(int r, int g, int b, int a);
		Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
		void Clamp();
		// Operator Overloads
		Color operator* (const Color &c) const;
		Color operator* (float f) const;
		Color operator+ (const Color &c) const;
		void operator+= (const Color &c);
		Color operator/ (float f) const;
	};

	float ClampZeroOne(float n);
	int ClampTwoFiveFive(int n);
	void Clamp(float &v);

	// Color Functions
	Color ConvertColor(unsigned int pixel);
	Color ConvertColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	void ConvertColor(Color color, unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a);
	unsigned int ConvertColor (Color color);
	void ExtractColor(unsigned int pixel, unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a);
	unsigned int CreateColor(int r, int g, int b, int a);

	// Pixel Functions
	unsigned int GetPixel (int* video, int x, int y, int w, int h);
	void PutPixel(int* video, int x, int y, int w, int h, unsigned int color);														
	void PutPixel(const Texture& texture, int x, int y, unsigned int color);
	unsigned int GetScaledPixel(int* video, int nWidth, int nHeight, double fStretchWidth, double fStretchHeight, int x, int y);	// ERROR CHECK
	void PutAlphaPixel(int* video, int nWidth, int nHeight, int x, int y, unsigned int color);										// ERROR CHECK
	void PutAlphaPixel(int* video, int nWidth, int nHeight, int x, int y, unsigned int color, double fGlobalAlpha);					// ERROR CHECK

	// Color Blending Functions
	unsigned int BlendColor(unsigned int dest, unsigned int src, double fGlobalAlpha);												// ERROR CHECK
	unsigned int BlendColor(unsigned int dest, unsigned int src);																	// ERROR CHECK
	Color BlendColorMultiply(const Color& c1, const Color& c2);																		// ERROR CHECK
	Color BlendColorAdd(const Color& c1, const Color& c2);																			// ERROR CHECK
	Color BlendColorAverage(const Color& c1, const Color& c2);																		// ERROR CHECK

	// Map functions
	Color GetColorFromMap(float tu, float tv, Texture* pTextureMap);
	Color FromIntARGB(int pixel);
	Vector GetNormalFromMap(float tu, float tv, Texture* pTextureMap);
	float GetValueFromMap(float tu, float tv, Texture* pTextureMap);

	// Operator Overloads
	Color operator* (float f, const Color &c);

	void ToonRamp(float &z);
}

