// Utility.h
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <GL/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <mutex>

#include "RayTrace.h"
#include "CORE.File.Bmp.h"
#include "Debug.ErrorLog.h"

#define MAX_RENDERING_LINES 16

extern int* VideoMemory;
extern int linesCompleted;			// Used to detect the number of completed lines
extern int currentLinesRendering;	// Number of lines that are currently rendering
extern int nextLine;				// The next line that needs to be rendered

extern List<RenderRegion*> regions;
extern List<RenderRegion*> currentRenders;

extern int VIEWPORT_WIDTH;
extern int VIEWPORT_HEIGHT;
extern ofstream dataLog;
//extern ofstream errorLog;

extern mutex currentRegion_MUTEX;
extern mutex render_MUTEX;
extern clock_t renderTime;
extern bool finished;

GLuint GL_CreateTexture(int width, int height);
void GL_UpdateTexture(GLuint texture, int width, int height, int* data);
void GL_DeleteTexture(GLuint texture);
void GL_Initialize(HWND hWnd, HDC* hDC, HGLRC* hRC);
void GL_Uninitialize(HWND hWnd, HDC hDC, HGLRC hRC);

void OnInitialize();
void Uninitialize();

DWORD WINAPI RenderWorkThread (LPVOID lpThreadParameter);
void OnFrame(RenderRegion* region);

bool OpenDataLog();
bool WriteCloseDataLog();

void OutputRenderTime(ofstream &dataLog, char* timeStamp,/* char* endDateTime,*/ clock_t renderTime);
void CalculateOutputTime(ofstream &dataLog, clock_t renderTime);
//void CreateFileBMP(time_t currentTime, char* name, char* endDateTime);
void CreateFileBMP(char* name, char* timeStamp);
void RenderScene(MSG &msg, BOOL &bQuit, GLuint &texture, HDC &hDC/*, time_t &currentTime*/);
void ManageThreads();
void RenderLine(RenderRegion* region);

void DoMultiThreading(MSG &msg, BOOL &bQuit, GLuint &texture, HDC &hDC);
void DoEnhancedMultiThreading(MSG &msg, BOOL &bQuit, GLuint &texture, HDC &hDC);
void SaveCurrentDateTime(char* msg);