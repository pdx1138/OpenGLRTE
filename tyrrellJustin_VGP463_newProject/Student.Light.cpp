// Student.Light.cpp
///////////////////////////////////////////////////////////////////////////////////////////////
#include "Student.Light.h"

Light::Light() {
	this->type = DIR_LIGHT;
	this->direction = Vector(0.0f, 0.0, 1.0f, 1.0f);
	this->intensity = 1.0f;
	this->origin = Point();
	this->color = Color(255.0f, 255.0f, 255.0f, 255.0f);
	this->specularColor = Color(255.0f, 255.0f, 255.0f, 255.0f);
	this->specularIntensity = 0.0f;
	this->specularPower = 500.0f;
}

Light::Light(LightType type, Point origin, Vector direction, Color color) {
	this->type = type;
	this->direction = direction;
	this->intensity = 1.0f;
	this->origin = origin;
	this->color = color;
	this->specularColor = Color(255.0f, 255.0f, 255.0f, 255.0f);
	this->specularIntensity = 0.0f;
	this->specularPower = 500.0f;
}

void Light::SetIntensity(float f) {
	this->intensity = f;
	if(this->intensity > 1.0f) this->intensity = 1.0f;
	if(this->intensity < 0.0f) this->intensity = 0.0f;
}

void Light::SetSpecularAttributes(float specPower, Color specColor) {
	this->specularPower = specPower;
	this->specularColor = specColor;
}