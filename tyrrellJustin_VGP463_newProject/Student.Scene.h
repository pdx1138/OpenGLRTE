// Student.Scene.h
///////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Student.List.h"
//#include "Student.Object.h"
//#include "Student.Light.h"

class Object;
class Light;

class Scene {
	public:
		List<Object*> objects;
		List<Light*> lights;
};