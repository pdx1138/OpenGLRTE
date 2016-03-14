// main.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "main.h"

// WinMain //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;

	GLuint texture;

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "VGP463WndClass";
	RegisterClass(&wc);

	// create main window
	int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	hWnd = CreateWindow(
		"VGP463WndClass",								// Identifier of registered window class
		"VGP463 CG Programming Adv Topics",				// Window Title
		WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,		// Window Properties
		screenWidth / 2.0f - VIEWPORT_WIDTH / 2.0f,		// X Position
		screenHeight / 2.0f - VIEWPORT_HEIGHT / 2.0f,	// Y Position
		VIEWPORT_WIDTH, VIEWPORT_HEIGHT,				// Dimensions
		NULL, NULL, hInstance, NULL);					// Misc. settings

	GL_Initialize(hWnd, &hDC, &hRC);
	// Initialize the default (teture)
	texture = GL_CreateTexture(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	// Initialize (VideoMemory)
	VideoMemory = new int [VIEWPORT_WIDTH * VIEWPORT_HEIGHT];
	// Initialize name for image save
	char* name = (char*)malloc(sizeof(char) * FILE_NAME_SIZE);
	for(int n = 0; n < FILE_NAME_SIZE; ++n) {
		name[n] = '/0';
	}
	
	// Open the Data Log
	/*ofstream dataLog;*/
	dataLog.open("DataLog.txt", ios::app);
	/*errorLog.open("ErrorLog.txt", ios::app);*/

	// Begin tracking the time
	renderTime = clock();
	SaveCurrentDateTime("\nStart Time: ");
	/*time_t currentTime = 0;
	currentTime = time(0);
	char* startDateTime = ctime(&currentTime);
	dataLog << endl << "Start Time: " << startDateTime;*/

	#ifdef ENABLE_MULTI_THREADING
		DoMultiThreading(msg, bQuit, texture, hDC);
	#else
		#ifdef ENABLE_ENHANCED_MULTI_THREAD
			DoEnhancedMultiThreading(msg, bQuit, texture, hDC);
		#else
			RenderRegion* regionFull = new RenderRegion(0.0f, 0.0f, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

			regionFull->active = true;
			regions.AddValue(regionFull);

			CreateThread(NULL, 0, RenderWorkThread, (LPVOID) regionFull, 0, NULL);

			RenderScene(msg, bQuit, texture, hDC);
			//currentTime = time(0);
		#endif
	#endif

	// TODO: Move (renderTime) to Manage Threads Function so we have as accurate of a time as possible
	//renderTime = clock() - renderTime;

	//currentTime = time(0);
	//currentTime = 0;
	//time(&currentTime);
	//char* endDateTime = ctime(&currentTime);
	//dataLog << "End Time: " << endDateTime;

	time_t currentTime = time(0);
	char* saveDateTime = ctime(&currentTime);
	
	//CreateFileBMP(currentTime, name, saveDateTime);
	CreateFileBMP(name, saveDateTime);
	OutputRenderTime(dataLog, saveDateTime,/* endDateTime,*/ renderTime);
	
	// free the Texture
	GL_DeleteTexture(texture);

	// shutdown OpenGL
	GL_Uninitialize(hWnd, hDC, hRC);

	// destroy the window
	DestroyWindow(hWnd);

	ExitProcess(0);

	free(name);

	return 0;
}

// Window Proc ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_DESTROY:
			return 0;
		case WM_KEYDOWN:
			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
				case VK_RETURN:
					return 0;
				case VK_F1:
					PostQuitMessage(0);
					return 0;
			}
			
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}