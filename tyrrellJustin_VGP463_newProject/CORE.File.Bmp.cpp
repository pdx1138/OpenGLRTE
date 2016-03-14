// CORE.File.Bmp.cpp
///////////////////////////////////////////////////////////////////////////////////////////////
#include "CORE.File.Bmp.h"

BMP_FILE::BMP_FILE() {
	Init();
}

BMP_FILE::BMP_FILE(LPSTR szFilename) {
	this->m_bIsValid = Load(szFilename);
}

BMP_FILE::~BMP_FILE() {
	Destroy();
}

void BMP_FILE::Create(int* video, int width, int height) {
	this->m_bmFileHeader.bfType = MAKEWORD('B','M');
	this->m_bmFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
	this->m_bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	this->m_bmFileHeader.bfReserved1 = 0;
	this->m_bmFileHeader.bfReserved2 = 0;

	this->m_bmInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->m_bmInfoHeader.biPlanes = 1;
	this->m_bmInfoHeader.biXPelsPerMeter = (LONG)(600 / 39.3701f); // 600 Dpi
	this->m_bmInfoHeader.biYPelsPerMeter = (LONG)(600 / 39.3701f); // 600 Dpi
	this->m_bmInfoHeader.biClrUsed = 0;
	this->m_bmInfoHeader.biClrImportant = 0;
	this->m_bmInfoHeader.biBitCount = 32;
	this->m_bmInfoHeader.biCompression = BI_RGB;
	this->m_bmInfoHeader.biHeight = height;
	this->m_bmInfoHeader.biWidth = width;

	this->m_bmInfoHeader.biSizeImage = this->m_bmInfoHeader.biHeight * this->m_bmInfoHeader.biWidth * this->m_bmInfoHeader.biBitCount / 8;

	this->m_pRGB = (LPRGB)video;
}

bool BMP_FILE::Load(LPSTR szFilename) {
	HANDLE hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if(!hFile) return false;
	Load(hFile);
	CloseHandle(hFile);
	return m_bIsValid;
}

bool BMP_FILE::Load(HANDLE hFile) {
	Destroy();

	DWORD dwBytesRead;
	int bSuccess;

	DWORD &biSizeImage = m_bmInfoHeader.biSizeImage;

	// File Header
	bSuccess = ReadFile(hFile, (LPVOID)&this->m_bmFileHeader, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
	if(!(bSuccess && this->m_bmFileHeader.bfType == MAKEWORD('B', 'M'))) {
		ASSERT(false); 
		return false;
	}

	// Bitmap Info Header
	bSuccess = ReadFile(hFile, (LPVOID)&m_bmInfoHeader, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if(!(bSuccess && this->m_bmInfoHeader.biBitCount == 32 && this->m_bmInfoHeader.biCompression == BI_RGB)) {
		ASSERT(false);
		return false;
	}

	// This member may be zero since biCompression == BI_RGB
	if(!this->m_bmInfoHeader.biSizeImage) {
		this->m_bmInfoHeader.biSizeImage = this->m_bmInfoHeader.biHeight * this->m_bmInfoHeader.biWidth * this->m_bmInfoHeader.biBitCount / 8;
	}

	// Image Data
	this->m_pRGB = (LPRGB)new BYTE[biSizeImage];
	if(!this->m_pRGB){
		ASSERT(false);
		return false;
	}

	bSuccess = ReadFile(hFile, (LPVOID)this->m_pRGB, biSizeImage, &dwBytesRead, NULL);
	if(!bSuccess) {
		ASSERT(false);
		return false;
	}

	this->m_nNumPixels = biSizeImage / 4;

	this->m_bIsValid = true;
	return this->m_bIsValid;
}

bool BMP_FILE::Save(char* szFilename) {
	HANDLE hFile = CreateFile((LPSTR)szFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if(!hFile) {
		ASSERT(false);
		return false;
	}
	SaveHandle(hFile);
	CloseHandle(hFile);
	return this->m_bIsValid;
}

void BMP_FILE::SaveHandle(HANDLE hFile) {
	DWORD dwBytesWrit;
	int bSuccess;

	DWORD &biSizeImage = this->m_bmInfoHeader.biSizeImage;

	// File Header
	bSuccess = WriteFile(hFile, (LPVOID)&this->m_bmFileHeader, sizeof(BITMAPFILEHEADER), &dwBytesWrit, NULL);
	if(!(bSuccess && this->m_bmFileHeader.bfType == MAKEWORD('B','M'))) {
		ASSERT(false);
		return;// false;
	}

	// This member may be zero since biCompression == BI_RGB
	if(!this->m_bmInfoHeader.biSizeImage) {
		this->m_bmInfoHeader.biSizeImage = this->m_bmInfoHeader.biHeight * this->m_bmInfoHeader.biWidth * this->m_bmInfoHeader.biBitCount / 8;
	}

	// Bitmap Info Header
	bSuccess = WriteFile(hFile, (LPVOID)&this->m_bmInfoHeader, sizeof(BITMAPINFOHEADER), &dwBytesWrit, NULL);
	if(!(bSuccess && this->m_bmInfoHeader.biBitCount == 32 && this->m_bmInfoHeader.biCompression == BI_RGB)) {
		ASSERT(false);
		return;// false;
	}

	// Image Data
	if(!this->m_pRGB) {
		ASSERT(false);
		return;// false;
	}

	// Wrtie file in chunks of 5Mb
	DWORD nChunkSize = 5 * 1024 * 1024;
	DWORD nNumBytesWritten = 0;
	DWORD nRemaining = biSizeImage;
	while(nRemaining > 0) {
		if(nRemaining < nChunkSize) {
			nChunkSize = nRemaining;
		}

		BOOL bResult = WriteFile(hFile, (LPVOID)(((BYTE*)this->m_pRGB) + nNumBytesWritten), nChunkSize, &dwBytesWrit, NULL);
		nNumBytesWritten += dwBytesWrit;
		nRemaining -= dwBytesWrit;
		if(bResult == false) {
			break;
		}

	}
	this->m_bIsValid = true;
	//return this->m_bIsValid;
}



LPRGB BMP_FILE::GetRGB() {
	return m_pRGB;
}

bool BMP_FILE::IsValid() {
	return m_bIsValid;
}

DWORD BMP_FILE::GetNumPixels() {
	return m_nNumPixels;
}
		
BOOL BMP_FILE::Maskify(PIXEL rgbBkgColor) { 
	if(!m_bIsValid) return FALSE;

	for (int n = 0; n < m_nNumPixels; ++n) {
		PIXEL &rgb = m_pRGB[n];

		if(rgb.red == rgbBkgColor.red && rgb.green == rgbBkgColor.green && rgb.blue == rgbBkgColor.blue) {
			rgb.red = 255;
			rgb.green = 255;
			rgb.blue = 255;
		} else {
			rgb.red = 0;
			rgb.green = 0;
			rgb.blue = 0;
		}
	}
	return TRUE;
}

BOOL BMP_FILE::BlueScreen(PIXEL rgbSetTo, PIXEL rgbThreshold) { 
	if(m_bIsValid) return FALSE;

	for(int n = 0; n < m_nNumPixels; ++n) {
		PIXEL &rgb = m_pRGB[n];
		if(rgb.red < rgbThreshold.red && rgb.green < rgbThreshold.green && rgb.blue < rgbThreshold.blue) {
			rgb.red = rgbSetTo.red;
			rgb.green = rgbSetTo.green;
			rgb.blue = rgbSetTo.blue;

		}
	}

	return TRUE;
}

HBITMAP BMP_FILE::CreateHandle(HDC hDC) { 
	LPBYTE pBits;
	HBITMAP hBitmap = CreateDIBSection(hDC, (LPBITMAPINFO) &m_bmInfoHeader, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0);

	if(!hBitmap) return NULL;

	memcpy(pBits, m_pRGB, m_bmInfoHeader.biSizeImage);

	return hBitmap;
}

void BMP_FILE::ReleaseHandle(HBITMAP hBmp){ 
	DeleteObject((HGDIOBJ)hBmp);
}

void BMP_FILE::Init() {
	m_bIsValid = false;
	memset(&m_bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&m_bmInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	m_pRGB = NULL;
	m_nNumPixels = 0;
}

void BMP_FILE::Destroy() {
	//if(m_pRGB) delete [] m_pRGB;
	Init();
}

int BMP_FILE::GetSize() {
	return m_bmInfoHeader.biSizeImage;
}

int BMP_FILE::GetBpp() {
	return m_bmInfoHeader.biBitCount;
}

int BMP_FILE::Width()  {
	return m_bmInfoHeader.biWidth;
}

int BMP_FILE::Height()  {
	return m_bmInfoHeader.biHeight;
}