// Student.Ray.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include "Student.Object.h"
#include "Student.Color.h"
#include "Student.Point.h"
#include "Student.List.h"
#include "Student.Light.h"

#include "Student.Scene.h"

#include <math.h>

class Object;
class SphereObject;

class Ray {
	public:
		Point origin;
		Vector direction;
		
		Ray();
		Point GetPointAt(float t) const;
};

class BaryCoord {
	public:
		float alpha, beta, gamma;
		float Calculate(float a, float b, float c) const;
};

class RenderRegion {
	public:
		float x1, x2, y1, y2;
		bool complete, active;

		RenderRegion();
		RenderRegion(float x1, float y1, float x2, float y2);
};

class HitInfo {
	public:
		float t;
		Point point;
		Ray ray;
		Vector normal;
};


class RayHitInfo {
	public:
		float t, tu, tv;
		Ray ray;
		Point point;
		unsigned int pixel;
		Vector normal;
		Vector tangent;
		Vector reflection;
		Vector refraction;
		Object* pHitObject;
		BaryCoord baryCoord;
		RayHitInfo();
};

bool GetNearestHit(const Ray &ray, Object* obj, RayHitInfo &hitInfo);
bool GetNearestHit(const Ray &ray, List<Object*> &objList, RayHitInfo &hitInfo);
bool GetNearestHit(const Ray &ray, Scene* scene, RayHitInfo &hitInfo);
bool GetNearestSphereHit(const Ray &ray, SphereObject* obj, RayHitInfo &hitInfo);
bool GetNearestSphereHit(const Ray &ray, List<Object*> &objList, RayHitInfo &hitInfo);