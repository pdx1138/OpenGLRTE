// Student.Point.cpp
///////////////////////////////////////////////////////////////////////////////////////////////

#include "Student.Point.h"
#include <math.h>

Point::Point() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 1.0f;
}

Point::Point(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Point::Normalize() {
	float d = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	this->x = x / d;
	this->y = y / d;
	this->z = z / d;
}

float Point::Length() {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}


float DotProduct(Vector a, Vector b) {
	return (a.x * b.x) + (a.y *  b.y) + (a.z * b.z);
}

// ADDITION OPERATOR OVERLOAD
Point Point::operator+(const Point &p) const {
	Point ret;

	ret.x = this->x + p.x;
	ret.y = this->y + p.y;
	ret.z = this->z + p.z;
	ret.w = ( this->w < p.w ) ? p.w : this->w;
	
	return ret;
}

Point Point::operator-(const Point &p) const {
	Point ret;

	ret.x = this->x - p.x;
	ret.y = this->y - p.y;
	ret.z = this->z - p.z;
	ret.w = ( this->w < p.w ) ? p.w : this->w;
	
	return ret;
}

Point operator*(float a, const Point &p) {
	Point result;

	result.x = a * p.x;
	result.y = a * p.y;
	result.z = a * p.z;
	result.w = p.w;

	return result;
}

Point Reflection(const Point &v, const Point &n) {
	return v - 2 * (DotProduct(v, n)) * n;
}

Point CrossProduct(const Point & v1, const Point &v2) {
	Point result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}