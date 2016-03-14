// Utility.cpp
/////////////////////////////////////////////////////////////////////////////////////

#include "Utility.h"

int* VideoMemory = nullptr;
int linesCompleted = 0;
int currentLinesRendering = 0;
int nextLine = 0;

List<RenderRegion*> regions;
List<RenderRegion*> currentRenders;
ofstream dataLog;

List<DEBUG::ErrorLog::ERROR_LOG> buddy;

mutex currentRegion_MUTEX;
mutex render_MUTEX;
clock_t renderTime;
bool finished = false;

using namespace COLOR;
using namespace DEBUG;

#ifdef VGA
	int VIEWPORT_WIDTH = 640;
	int VIEWPORT_HEIGHT = 480;
#else
	#ifdef HD_720
		int VIEWPORT_WIDTH = 1280;
		int VIEWPORT_HEIGHT = 780;
	#else
		#ifdef HD_1080
			int VIEWPORT_WIDTH = 1920;
			int VIEWPORT_HEIGHT = 1080;
		#else
			int VIEWPORT_WIDTH = 640;
			int VIEWPORT_HEIGHT = 480;
		#endif
	#endif
#endif

void OnInitialize() {
	srand(time(NULL));
	scene = GetNewScene();

	// Initialize Camera /////////////////////////////////////////////////////
	
	#ifdef USE_ORTHO_CAMERA
		FIXED_ORTHO_CAMERA* cam = new FIXED_ORTHO_CAMERA();
		cam->Initialize();
		// cam-> ...
	#else
	#ifdef USE_PERSP_CAMERA
		FIXED_PERSP_CAMERA* cam = new FIXED_PERSP_CAMERA();
		cam->Initialize();
		#ifdef USE_PERSP_LOOK_AT
			cam->LookAt(0.0f, 0.0f, 1500.0f);
		#endif
		// cam-> ...
	#endif
	#endif

	mainCamera = cam;

	textureTest = new Texture();
	textureTest->LoadTexture("text1.bmp");
	
	// Initialize Objects /////////////////////////////////////////////////////
	// RED Sphere
	SphereObject* sphere = new SphereObject();
	sphere->radius = 100.0f;
	sphere->position.x = 350.0f;
	sphere->position.y = 125.0f;
	sphere->position.z = 1200.0f;
	sphere->color = Color( 1.0f, 0.0f, 0.0f, 1.0f);
	sphere->texture.LoadTexture("text6.bmp");
	
	// GREEN Sphere
	SphereObject* sphere2 = new SphereObject();
	sphere2->radius = 100.0f;
	sphere2->position.x = 175.0f;
	sphere2->position.y = -150.0f;
	sphere2->position.z = 1200.0f;
	sphere2->color = Color( 0.0f, 1.0f, 0.0f, 1.0f);
	sphere2->texture.LoadTexture("text5.bmp");

	// BLUE Sphere
	SphereObject* sphere3 = new SphereObject();
	sphere3->radius = 100.0f;
	sphere3->position.x = 525.0f;
	sphere3->position.y = -150.0f;
	sphere3->position.z = 1200.0f;
	sphere3->color = Color( 0.0f, 0.0f, 1.0f, 1.0f);
	sphere3->texture.LoadTexture("text4.bmp");

	// BLACK Sphere
	SphereObject* sphere4 = new SphereObject();
	sphere4->radius = 50.0f;
	sphere4->position.x = -400.0f;
	sphere4->position.y = 50.0f;
	sphere4->position.z = 1200.0f;
	sphere4->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	sphere4->texture.LoadTexture("text8.bmp");

	SphereObject* sphere5 = new SphereObject();
	sphere5->radius = 50.0f;
	sphere5->position.x = 350.0f;
	sphere5->position.y = -66.0f;
	sphere5->position.z = 1200.0f;
	sphere5->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	sphere5->texture.LoadTexture("text8.bmp");

	// RED Tri
	TriangleObject* tri1 = new TriangleObject(Point(-550.0f, -25.0f, 1200.0f, 1.0f), 
											  Point(-400.0f, -225.0f, 1200.0f, 1.0f), 
											  Point(-250.0f, -25.0f, 1200.0f, 1.0f));
	tri1->color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	tri1->normal = -1.0f * tri1->normal;
	tri1->texture.LoadTexture("text2.bmp");

	// GREEN Tri
	TriangleObject* tri2 = new TriangleObject(Point(-700.0f, 175.0f, 1200.0f, 1.0f), 
											  Point(-550.0f, -25.0f, 1200.0f, 1.0f), 
											  Point(-400.0f, 175.0f, 1200.0f, 1.0f));
	tri2->color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	tri2->normal = -1.0f * tri2->normal;
	tri2->texture.LoadTexture("text3.bmp");

	// BLUE Tri
	TriangleObject* tri3 = new TriangleObject(Point(-400.0f, 175.0f, 1200.0f, 1.0f), 
											  Point(-250.0f, -25.0f, 1200.0f, 1.0f), 
											  Point(-100.0f, 175.0f, 1200.0f, 1.0f));
	tri3->color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	tri3->normal = -1.0f * tri3->normal;
	tri3->texture.LoadTexture("text7.bmp");

	// WHITE Triangle
	TriangleObject* tri4 = new TriangleObject(Point(250.0f, -87.5f, 1225.0f, 1.0f), 
											  Point(325.0f, 12.5f, 1200.0f, 1.0f), 
											  Point(400.0f, -87.5f, 1175.0f, 1.0f));
	tri4->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	tri4->texture.LoadTexture("text8.bmp");

	// PLANE ///////////////////////////////////////////////////////////////////////////////////////////////////
	PlaneObject* plane = new PlaneObject(TriangleObject(Point(-2000.0f, -2000.0f, 2000.0f, 0.0f),
														Point(-2000.0f, 2000.0f, 2000.0f, 0.0f), 
														Point(2000.0f, -2000.0f, 2000.0f, 0.0f))
						 );
	plane->SetColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
	plane->SetTexture("PlaneText_2048x2048.bmp");
	plane->triLeft.reflectivity = 0.0f;
	plane->triRight.reflectivity = 0.0f;
	plane->reflectivity = 0.0f;
	
	PlaneObject* plane2 = new PlaneObject(TriangleObject(Point(-2000.0f, 400.0f, -2000.0f, 0.0f),
														 Point(2000.0f, 400.0f, 2000.0f, 0.0f), 
														 Point(-2000.0f, 400.0f, 2000.0f, 0.0f))
						 );
	plane2->SetColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
	plane2->SetTexture("PlaneText2_2048x2048.bmp");
	plane2->reflectivity = 0.0f;

	PlaneObject* plane3 = new PlaneObject(TriangleObject(Point(-2000.0f, -400.0f, -2000.0f, 0.0f),
														 Point(-2000.0f, -400.0f, 2000.0f, 0.0f), 
														 Point(2000.0f, -400.0f, -2000.0f, 0.0f))
						 );
	plane3->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
	plane3->reflectivity = 0.0f;

	PlaneObject* plane4 = new PlaneObject(TriangleObject(Point(-2000.0f, -2000.0f, -2000.0f, 0.0f),
														 Point(-2000.0f, 2000.0f, -2000.0f, 0.0f), 
														 Point(2000.0f, -2000.0f, -2000.0f, 0.0f))
						 );
	plane4->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
	
	// Form a right triangle, or half a plane    b |\
	//											   | \
	//											 a |__\ c
	//
	//TriangleObject* triPlane1 = new TriangleObject(Point(-400.0f, -300.0f, 200.0f, 0.0f), // a			
	//											   Point(-400.0f, 300.0f, 600.0f, 0.0f),  // b						
	//											   Point(400.0f, -300.0f, 200.0f, 0.0f)); // c															
	//
	// Form the other half of the plane			
	// where a = b of first triangle 				__
	// where c = c of first triangle			 a \  | b
	// where b.x = c.x of first triangle		    \ |
	// where b.y = b.y of first triangle		     \| c
	// where b.z = b.z of first triangle
	// To ensure a perfectly rectangular plane
	//
	//TriangleObject* triPlane2 = new TriangleObject(triPlane1->b,  // a
	//											   Point(triPlane1->c.x, triPlane1->b.y, triPlane1->b.z, 0.0f),  // b
	//											   triPlane1->c);  // c
	//triPlane1->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	//triPlane2->color = triPlane1->color;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Initialize Lighting /////////////////////////////////////////////////////
	Light* light = new Light(POINT_LIGHT, Point(100.0f, 100.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
	//Light* light = new Light(POINT_LIGHT, Point(0.0f, 0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
	light->SetIntensity(1.0f);
	light->SetSpecularAttributes(60.0f, Color(1.0f, 1.0f, 1.0f, 1.0f));
	light->direction.Normalize();

	Light* dirLight = new Light(DIR_LIGHT, Point(0.0f, 0.0f, 0.0f, 0.0f), Vector(0.0f, 5.0f, -5.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
	dirLight->SetIntensity(1.0);
	dirLight->SetSpecularAttributes(0.0f, Color(0.0f, 0.0f, 0.0f, 1.0f));
	dirLight->direction.Normalize();

	Light* dirLight2 = new Light(DIR_LIGHT, Point(0.0f, 0.0f, 0.0f, 0.0f), Vector(0.0f, -5.0f, -5.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
	dirLight->SetIntensity(1.0);
	dirLight->SetSpecularAttributes(0.0f, Color(0.0f, 0.0f, 0.0f, 1.0f));
	dirLight->direction.Normalize();
	////////////////////////////////////////////////////////////////////////////
	
	// Add Objects to the Scene ////////////////////////////////////////////////
	scene->objects.AddValue(sphere);
	scene->objects.AddValue(sphere2);
	scene->objects.AddValue(sphere3);
	scene->objects.AddValue(sphere4);
	scene->objects.AddValue(sphere5);
	scene->objects.AddValue(plane);
	scene->objects.AddValue(plane2);
	scene->objects.AddValue(plane3);

	scene->objects.AddValue(tri1);
	scene->objects.AddValue(tri2);
	scene->objects.AddValue(tri3);
	////////////////////////////////////////////////////////////////////////////

	// Add Lights to the Scene /////////////////////////////////////////////////
	scene->lights.AddValue(light);	
	scene->lights.AddValue(dirLight);
	scene->lights.AddValue(dirLight2);
	////////////////////////////////////////////////////////////////////////////

	finished = false;

}

void Uninitialize() {
	// Delete Global pointers
	int length = scene->objects.length;
	for (int n = 0; n < length; ++n) {
		delete scene->objects[n];
	}
	
	delete textureTest;
	delete mainCamera;
	delete VideoMemory;
}

DWORD WINAPI RenderWorkThread (LPVOID lpThreadParameter) {
	//render_MUTEX.lock();
	RenderRegion* region = (RenderRegion*)lpThreadParameter;
	OnFrame(region);
	region->complete = true;
	//render_MUTEX.unlock();
	return 0;
}

void OnFrame(RenderRegion* region) {
	RayTrace(region, VideoMemory, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, mainCamera, scene, RAY_RECURSSION_OFFSET, false);
}

void OutputRenderTime(ofstream &dataLog, char* saveDateTime,/* char* endDateTime,*/ clock_t renderTime) {
	//dataLog << endl << "Start Time: " << startDateTime;

	dataLog << "Save Time Stamp: " << saveDateTime;
	dataLog << "\n\tNumber of Lights in Scene: " << scene->lights.length << endl;
	dataLog << "\tNumber of Objects in Scene: " << scene->objects.length << endl;
	
	#ifdef VGA
		dataLog << "\tResolution: VGA\n" << endl;
	#else
		#ifdef HD_720
			dataLog << "\tResolution: HD 720\n" << endl;
		#else
			#ifdef HD_1080
				dataLog << "\tResolution: HD 1080\n" << endl;
			#endif
		#endif
	#endif

	#ifdef USE_ORTHO_CAMERA
		dataLog << "\tOrthographic Camera\n";
	#else USE_PERSP_CAMERA
		dataLog << "\tPerspective Camera\n";
		#ifdef USE_PERSP_LOOK_AT
		dataLog << "\tPerspective Look At\n";
		#endif
	#endif
	
	#ifdef ENABLE_SHADOWS
		dataLog << "\tBasic Shadows\n";
	#else ENBALE_SOFTSHADOWS
		dataLog << "\tSoft Shadows\n";
		dataLog << "\t\tShadow Rays: " << NUM_SHADOW_RAYS << endl;
	#endif
	
	#ifdef ENABLE_SPECULAR_LIGHTING
		dataLog << "\tSpecular Lighting\n";
	#endif
	
	#ifdef ENABLE_REFLECTIONS
		dataLog << "\tReflections\n";
		dataLog << "\t\tReflection Recurssion Depth: " << (RAY_RECURSSION_DEPTH - RAY_RECURSSION_OFFSET + 1) << endl;
	#endif
	
	#ifdef ENABLE_TEXTURE_MAPPING
		dataLog << "\tTexture Mapping\n";
	#endif
	
	#ifdef ENABLE_AMBIENT_OCCLUSION
		dataLog << "\tAmbient Occlusion\n";
		dataLog << "\t\tAmbient Occlusion Window: " << MAX_ABM_OCC_WINDOW << endl;
		dataLog << "\t\tAmbient Occlusion Length: " << MAX_AMB_OCC_LENGTH << endl;
	#endif
	
	#ifdef ENABLE_FRESNEL_LIGHT
		dataLog << "\tFresnel Lighting\n";
	#endif
	
	#ifdef ENABLE_TOON_SHADING
		dataLog << "\tToon Shading\n";
	#endif
	
	#ifdef ENABLE_ANTI_ALIASING
		dataLog << "\tAnti-Aliasing\n";
		dataLog << "\t\tRandom Jitter Sampling\n";
		//dataLog << "\t\tNumber Of Jitter Samples :: X = " << xsample << " :: Y = " << num << "\n";
	#endif
	
	#ifdef ENABLE_MULTI_THREADING
		dataLog << "\tMultiThreading\n";
	#endif

	#ifdef ENABLE_ENHANCED_MULTI_THREAD
		dataLog << "\tEnhanced MultiThreading\n";
		dataLog << "\t\tMaximum Number of Threads: " << MAX_RENDERING_LINES << endl;
	#endif
	
	CalculateOutputTime(dataLog, renderTime);
}

//void CreateFileBMP(time_t currentTime, char* name, char* endDateTime) {
void CreateFileBMP(char* name, char* timeStamp) {
	Swizzle(VideoMemory, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	BMP_FILE bmp;
	bmp.Create(VideoMemory, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	
	//currentTime = time(0);
	strcpy(name, "Img_");
	strcat(name, timeStamp);
	int len = strlen(name);
	for(int n = len - 1; n >= 0;--n) {
		if(n > (len - 2)) {
			name[n] = '\0';
			continue;
		}
		if(name[n] == ':') name[n] = '-';
	}
	strcat(name, ".bmp");
	bmp.Save(name);
}

void CalculateOutputTime(ofstream &dataLog, clock_t renderTime) {
	float time = ((float)renderTime / CLOCKS_PER_SEC);

	int seconds = 0, minutes = 0, hours = 0;

	if(time > 0.0f) {
		seconds = (int)time % 60;
		time /= 60.0f;
	}

	if(time > 1.0f) {
		minutes = (int)time % 60;
		time /= 60.0f;
	}

	if(time > 1.0f) {
		hours = (int)time;
	}
	
	dataLog << "TotalTime >> H:M:S >> " << hours << ":" << minutes << ":" << seconds << endl;
}


GLuint GL_CreateTexture(int width, int height) {
	GLuint texture; 
	int* data;

	data = new int [width * height];

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete [] data;
	
	return texture;
}

void GL_UpdateTexture(GLuint texture, int width, int height, int* data) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void GL_DeleteTexture(GLuint texture) {
	glDeleteTextures(1, &texture);
}

VOID GL_Initialize(HWND hWnd, HDC* hDC, HGLRC* hRC) {
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	// Get the Device Context (DC)  /////////////////////////////////////////////////////
	*hDC = GetDC(hWnd);

	// Set the pixel format for the DC  /////////////////////////////////////////////////////
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, iFormat, &pfd);

	// create and enable the render context (RC)  /////////////////////////////////////////////////////
	*hRC = wglCreateContext(*hDC);
	wglMakeCurrent(*hDC, *hRC);

	// Initialize User Variables
	OnInitialize();
}

VOID GL_Uninitialize(HWND hWnd, HDC hDC, HGLRC hRC) {
	// Uninitialize User Variables
	Uninitialize();
	
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

void RenderScene(MSG &msg, BOOL &bQuit, GLuint &texture, HDC &hDC/*, time_t &currentTime*/) {
	while (!bQuit) {

	// TODO: Function for checking what has been rendered, and creating new threads when old threads finish.
	#ifdef ENABLE_ENHANCED_MULTI_THREAD
		#ifdef ENABLE_MULTI_THREADING
		#else
			if(finished == false) ManageThreads();
			//TODO: Need to account for ALL regions being rendered.  
			// check for last line being released, then do check for all.
			if(regions.GetValueAt(VIEWPORT_HEIGHT - 1)->complete == true) {
				// TODO: IF ALL LINES are complete, then finished
				finished = true;
				renderTime = clock() - renderTime;
				//SaveCurrentDateTime("End Time: ");
			}
		#endif
	#endif

	#ifdef ENABLE_MULTI_THREADING
		if(finished == false) {
			for(int n = 0; n < regions.length; ++n) {
				if(regions.GetValueAt(n)->complete == true) {
					if(n == regions.length - 1) {
						finished = true;
						renderTime = clock() - renderTime;
						//SaveCurrentDateTime("End Time: ");
					}
					else continue;

				}
				else break;
				/*finished = true;
				renderTime = clock() - renderTime;
				SaveCurrentDateTime("End Time: ");*/
			}
		}
	#else
		#ifdef ENABLE_ENHANCED_MULTI_THREAD
		#else
			if(finished == false) {
				if(regions.GetValueAt(0)->complete == true) {
					finished = true;
					renderTime = clock() - renderTime;
					//SaveCurrentDateTime("End Time: ");
				}
			}
		#endif
	#endif

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) {
				bQuit = true;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
		else {
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			GL_UpdateTexture(texture, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, VideoMemory);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);

			glPushMatrix();
			glBegin(GL_QUADS);
			
			glTexCoord2d(0.0f, 0.0f); glVertex2d(-1.0f, -1.0f);
			glTexCoord2d(1.0f, 0.0f); glVertex2d(+1.0f, -1.0f);
			glTexCoord2d(1.0f, 1.0f); glVertex2d(+1.0f, +1.0f);
			glTexCoord2d(0.0f, 1.0f); glVertex2d(-1.0f, +1.0f);

			glEnd();
			glPopMatrix();

			SwapBuffers(hDC);
		}
	}

	SaveCurrentDateTime("End Time: ");
}

//bool removeAt[MAX_RENDERING_LINES] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

void ManageThreads() {
	// Lock the Data so the threads cannot make any changes
	currentRegion_MUTEX.lock();

	ErrorLog::ERROR_LOG* errorLog = new ErrorLog::ERROR_LOG("ErrorLog.txt");

	// If the number of lines we are rendering are less than the number of
	// total lines we can render..
	if(currentRenders.length < MAX_RENDERING_LINES) {
		// Do we have more lines to render??
		if(nextLine < VIEWPORT_HEIGHT) {
			// Make a new thread
			while(currentRenders.length < MAX_RENDERING_LINES) {
				if(nextLine >= VIEWPORT_HEIGHT) break;
				RenderLine(regions.GetValueAt(nextLine));
				/*char* lineNum;
				itoa(nextLine,lineNum, 10);
				char* message = "New Line added " ;
				strcat(message, lineNum);
				errorLog->AddErrorMessage(message);*/
				errorLog->Message("New line added: ", nextLine);
				errorLog->Message("Num Renderers: ", currentRenders.length);
				++nextLine;
			}
		}
		else {
			if (linesCompleted == VIEWPORT_HEIGHT) {
				// We are done!!
				if(finished == false) {
					renderTime = clock() - renderTime;
					SaveCurrentDateTime("End Time: ");
					finished = true;
				}
			}
		}
	}

	/*if(currentRenders.length > MAX_RENDERING_LINES) {
		int x = 0;
	}*/


	//if(nextLine > 18) {
	//	int x = 0;
	//}

	//// Lock the Data so the threads cannot make any changes
	//currentRegion_MUTEX.lock();

	// Since each time we remove an item the list changes, we need a way
	// to remove all the items at that are completed. However, there is the
	// potential of removing the incorrect item once the list is changed,
	// so if we start from the back of the list, we should always be removing
	// the correct item, since removing from the front would cause items in
	// the back to move forward, thus moving completed lines from their 
	// recored place.
	for(int n = currentRenders.length - 1; n >= 0; --n) {
		if(currentRenders.GetValueAt(n)->complete == true) {
			int num = currentRenders.GetValueAt(n)->y1;
			currentRenders.RemoveAt(n);
			//removeAt[n] = true;
			++linesCompleted;

			errorLog->Message("**Removed line: ", num);
			errorLog->Message("Num Renderers: ", currentRenders.length);			
		}
	}	

	errorLog->PrintErrorLog();

	delete errorLog;
	// Unlock the Data so the threads can make changes
	currentRegion_MUTEX.unlock();
}

void RenderLine(RenderRegion* region) {
	if(region == nullptr) return;
	//currentRegion_MUTEX.lock();
	//render_MUTEX.lock();
	currentRenders.AddValue(region);
	//currentRegion_MUTEX.unlock();
	region->active = true;
	//render_MUTEX.unlock();
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region, 0, NULL);
}

void DoMultiThreading(MSG &msg, BOOL &bQuit, GLuint &texture, HDC &hDC) {
	// Prepare all 8 regions for rendering
	RenderRegion* region1 = new RenderRegion(0.0f, 0.0f,						  (VIEWPORT_WIDTH / 2.0f) - 1.0f, (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
	RenderRegion* region2 = new RenderRegion(0.0f, VIEWPORT_HEIGHT / 4.0f,	  (VIEWPORT_WIDTH / 2.0f) - 1.0f, 2 * (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
	RenderRegion* region3 = new RenderRegion(0.0f, 2 * VIEWPORT_HEIGHT / 4.0f, (VIEWPORT_WIDTH / 2.0f) - 1.0f, 3 * (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
	RenderRegion* region4 = new RenderRegion(0.0f, 3 * VIEWPORT_HEIGHT / 4.0f, (VIEWPORT_WIDTH / 2.0f) - 1.0f, 4 * (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
							    
	RenderRegion* region5 = new RenderRegion(VIEWPORT_WIDTH / 2.0f, 0.0,						   VIEWPORT_WIDTH, (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
	RenderRegion* region6 = new RenderRegion(VIEWPORT_WIDTH / 2.0f, VIEWPORT_HEIGHT / 4.0f,	   VIEWPORT_WIDTH, 2 * (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
	RenderRegion* region7 = new RenderRegion(VIEWPORT_WIDTH / 2.0f, 2 * VIEWPORT_HEIGHT / 4.0f, VIEWPORT_WIDTH, 3 * (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
	RenderRegion* region8 = new RenderRegion(VIEWPORT_WIDTH / 2.0f, 3 * VIEWPORT_HEIGHT / 4.0f, VIEWPORT_WIDTH, 4 * (VIEWPORT_HEIGHT / 4.0f) - 1.0f);
		
	//RenderRegion regionFull = RenderRegion();
	regions.AddValue(region1);
	regions.AddValue(region2);
	regions.AddValue(region3);
	regions.AddValue(region4);
	regions.AddValue(region5);
	regions.AddValue(region6);
	regions.AddValue(region7);
	regions.AddValue(region8);


	region1->active = true;
	region2->active = true;
	region3->active = true;
	region4->active = true;
	region5->active = true;
	region6->active = true;
	region7->active = true;
	region8->active = true;
			
	//OnFrame();
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region1, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region2, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region3, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region4, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region5, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region6, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region7, 0, NULL);
	CreateThread(NULL, 0, RenderWorkThread, (LPVOID) region8, 0, NULL);

	RenderScene(msg, bQuit, texture, hDC);
}

//List<RenderRegion*> regions;

void DoEnhancedMultiThreading(MSG &msg, BOOL &bQuit, GLuint &texture, HDC &hDC) {
	// TODO: Make a region for each line in advance and throw it in a list.
	for(int i = 0; i < VIEWPORT_HEIGHT; ++i) {
		regions.AddValue(new RenderRegion(0.0f, i, VIEWPORT_WIDTH, i));
	}
				
	RenderScene(msg, bQuit, texture, hDC);
}

void SaveCurrentDateTime(char* msg) {
	time_t currentTime = 0;
	currentTime = time(0);
	
	char* endDateTime = ctime(&currentTime);
	//dataLog << "End Time: " << endDateTime;
	dataLog << msg << endDateTime;
}