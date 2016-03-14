// RayTrace.h
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Student.Camera.h"
#include "Student.Scene.h"
#include "Student.List.h"
//#include <math>

#define RAY_RECURSSION_OFFSET 9

#define RAY_RECURSSION_DEPTH 10
#define NUM_SHADOW_RAYS 8 //128 //512 //16 //8 //256 //1024 //32
#define MAX_AMB_OCC_LENGTH 200.0f //400.0f //800.0f
#define MAX_ABM_OCC_WINDOW 20

// FEATURES /////////////////////////////////////////////////////////////////////////

// Screen Resolution
#define VGA
//#define HD_720
//#define HD_1080

// Camera Type
//#define USE_ORTHO_CAMERA
#define USE_PERSP_CAMERA
//#define USE_PERSP_LOOK_AT

// Shadow Type
//#define ENABLE_SHADOWS
#define ENBALE_SOFTSHADOWS

// Lighting Features
//#define ENABLE_SPECULAR_LIGHTING
#define ENABLE_REFLECTIONS
//#define ENABLE_AMBIENT_OCCLUSION
//#define ENABLE_FRESNEL_LIGHT
//#define ENABLE_TOON_SHADING

// Texture Features
//#define ENABLE_TEXTURE_MAPPING

// Various Enhancements
//#define ENABLE_ANTI_ALIASING
//#define ENABLE_MULTI_THREADING
#define ENABLE_ENHANCED_MULTI_THREAD
/////////////////////////////////////////////////////////////////////////////////////

extern Camera* mainCamera;
extern Scene* scene;

extern Texture* textureTest;

extern unsigned int nBgColor;

Scene* GetNewScene();

void CalculateLighting(Camera* pCam, Light* light, RayHitInfo &hitInfo, int recursionDepth, bool ambOcc = false);
void RayTrace (RenderRegion* region, int* vidMem, int VP_WIDTH, int VP_HEIGHT, Camera* pCam, Scene* scene, int recursionDepth, bool ambOcc);
void RayTrace (int* vidMem, int VP_WIDTH, int VP_HEIGHT, Camera* pCam, Scene* scene, int recursionDepth, bool ambOcc);
bool RayTrace(const Ray &ray, List<Object*> &objList, Light* light, RayHitInfo &hitInfo, int recursionDepth);

float RandFloat();
int Swizzle(int pixel);
void Swizzle(int* video, int width, int height);
int GetTexel(Texture* texture, float tu, float tv);