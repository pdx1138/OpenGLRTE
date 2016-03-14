// CORE.File.Bmp.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define POINTER_64 __ptr64
#include <Windows.h>

#ifndef ASSERT
#define ASSERT(e)
#endif

struct PIXEL {
	BYTE alpha, red, green, blue;

};

typedef PIXEL *LPRGB;

class BMP_FILE {
	public:
		int GetSize();
		int GetBpp();
		int Width();
		int Height();
		LPRGB m_pRGB;

		BMP_FILE();
		BMP_FILE(LPSTR szFilename);
		~BMP_FILE();

		void Create(int* video, int width, int height);

		bool Load(LPSTR szFilename);
		bool Load(HANDLE hFile);

		bool Save(char* szFilename);
		void SaveHandle(HANDLE hFile);

		LPRGB GetRGB();
		bool IsValid();
		DWORD GetNumPixels();
		
		BOOL Maskify(PIXEL rgbBkgColor);
		BOOL BlueScreen(PIXEL rgbSetTo, PIXEL rgbThreshold);

		HBITMAP CreateHandle(HDC hDC);
		void ReleaseHandle(HBITMAP hBmp);

	protected:
		int m_nNumPixels;
		void Init();
		void Destroy();

		bool m_bIsValid;

		BITMAPFILEHEADER m_bmFileHeader;
		BITMAPINFOHEADER m_bmInfoHeader;
		/*LPRGB m_pRGB;*/

	private:

};