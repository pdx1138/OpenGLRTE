// Student.Light.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Student.Point.h"
#include "Student.Color.h"

using namespace COLOR;

enum LightType {
	DIR_LIGHT = 0,
	POINT_LIGHT = 1,
	SPOT_LIGHT = 2
};

class Light {
	public:
		LightType type;
		float intensity;
		float specularIntensity;
		float specularPower;
		
		Point origin;
		Vector direction;
		
		Color color;
		Color specularColor;

		Light();
		Light(LightType type, Point origin, Vector direction, Color color);
		void SetIntensity(float f);
		void SetSpecularAttributes(float specPower, Color speColor);
};