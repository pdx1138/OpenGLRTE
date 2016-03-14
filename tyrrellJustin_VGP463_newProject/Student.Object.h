// Student.Object.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Student.Ray.h"
#include "Student.Point.h"
#include "Student.Color.h"
#include "Student.Light.h"

class Ray;
class HitInfo;
class RayHitInfo;

#define _DEFAULT_RADIUS 200.0f

class Object {
	public:
		COLOR::Color color;
		Texture texture;
		float reflectivity;

		Object();
		~Object();
		virtual bool DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo) = 0;
		//bool DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo);
};

class TextureCoord {
	public:
		float tu, tv;

		TextureCoord();
		TextureCoord(float tu, float tv);
};

class SphereObject : public Object {
	public:
		Point position;
		float radius;

		SphereObject();
		SphereObject(SphereObject* sphere);

		bool DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo);
};

class TriangleObject : public Object {
	public:
		Point a, b, c;
		TextureCoord auv, buv, cuv;
		Vector normal;

		TriangleObject();
		TriangleObject(Point a, Point b, Point c);
		TriangleObject(TriangleObject* tri);

		bool DoesRayHitYou(const Ray & ray, RayHitInfo &hitInfo);
		void CalculateNormal();
};

class PlaneObject : public Object {
	public:
		TriangleObject triLeft, triRight;
		Vector normal;

		PlaneObject();
		PlaneObject(Point a, Point b, Point c, Point d);
		PlaneObject(TriangleObject tri);
		PlaneObject(PlaneObject* plane);

		bool DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo);
		void SetColor(Color c);
		void SetTexture(char* fileName);
};

class CubeObject : public Object {
	public:

		CubeObject();

		bool DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo);
};