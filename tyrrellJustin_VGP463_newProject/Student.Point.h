// Student.Point.h
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class Point {
	public:
		float x, y, z, w;

		Point();
		Point(float x, float y, float z, float w);
		void Normalize();
		float Length();
		Point operator-(const Point &p) const;
		Point operator+(const Point &p) const;
		
};

typedef Point Vector;

float DotProduct(Vector a, Vector b);
Point operator*(float a, const Point &p);
Point Reflection(const Point &v, const Point &n);
Point CrossProduct(const Point & v1, const Point &v2);
