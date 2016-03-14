// Student.Ray.cpp
///////////////////////////////////////////////////////////////////////////////////////////////

#include "Student.Object.h"
#include <math.h>

Object::Object() {
	this->color = COLOR::Color( 0.9f, 0.2f, 0.85f, 1.0f);
	this->texture = Texture();
	this->reflectivity = 1.0f;
}

Object::~Object() {
	//delete this->texture;
}

//bool Object::DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo) {
//	return false;
//}

TextureCoord::TextureCoord() {
	this->tu = 0.0f;
	this->tv = 0.0f;
}

TextureCoord::TextureCoord(float tu, float tv) {
	this->tu = tu;
	this->tv = tv;
}

SphereObject::SphereObject() {
	this->position = Point();
	this->radius = _DEFAULT_RADIUS;
}

SphereObject::SphereObject(SphereObject* sphere) {
	this->color = sphere->color;
	this->position = sphere->position;
	this->radius = sphere->radius;
	this->texture = sphere->texture;
}

bool SphereObject::DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo) {
	// Prep variables for the equation:
	// d = b*b - 4.0 * a * c
	//////////////////////////////////////////////////////////////////////
	float a = DotProduct(ray.direction, ray.direction);
	
	// Holds the vector from the objects position to the ray's origin, 
	// then will hold the result of a dot product
	Vector p;	

	p = ray.origin - this->position;
	p = 2.0f * p;
	float b = DotProduct(p, ray.direction);

	p = ray.origin - this->position;
	float c = DotProduct(p, p) - this->radius * this->radius;
	
	// d = b*b - 4.0 * a * c
	float d = (float)(b * b - 4.0 * a * c);
	//////////////////////////////////////////////////////////////////////

	// Escape early
	if ( d < 0.0) return false;

	float e = sqrt(d);

	float t1 = (-b - e) / (2 * a);
	float t2 = (-b + e) / (2 * a);
	
	// t greaters greater than 0.0
	if(t1 < 0.0f) t1 = 0.0f;
	if(t2 < 0.0f) t2 = 0.0f;
	
	// Take the nearest of the two
	float t = (t1 < t2) ? t1 : t2;
	if( t <= 0.001f) return false;

	hitInfo.t = t;
	hitInfo.point = ray.GetPointAt(t);
	hitInfo.pHitObject = this;
	hitInfo.ray = ray;
	hitInfo.normal = hitInfo.point - this->position;
	hitInfo.normal.Normalize();
	hitInfo.tu = (hitInfo.normal.x / 2.0f) + 0.5f;
	hitInfo.tv = (hitInfo.normal.y / 2.0f) + 0.5f;
	return true;
}

TriangleObject::TriangleObject() {
	this->a = Point(-325.0f, 0.0f, 200.0f, 0.0f);
	this->b = Point(-300.0f, 200.0f, 200.0f, 0.0f);
	this->c = Point(-275.0f, 0.0f, 200.0f, 0.0f);

	this->auv = TextureCoord(0, 0);
	this->buv = TextureCoord(1, 0);
	this->cuv = TextureCoord(1, 1);
	
	this->CalculateNormal();
	//this->normal.Normalize();
}

TriangleObject::TriangleObject(Point a, Point b, Point c) {
	this->a = a;
	this->b = b;
	this->c = c;

	this->auv = TextureCoord(0, 0);
	this->buv = TextureCoord(1, 0);
	this->cuv = TextureCoord(1, 1);

	this->CalculateNormal();
	//this->normal.Normalize();
}

TriangleObject::TriangleObject(TriangleObject* tri) {
	this->a = tri->a;
	this->b = tri->b;
	this->c = tri->c;
	this->auv = tri->auv;
	this->buv = tri->buv;
	this->cuv = tri->cuv;
	this->color = tri->color;
	this->texture = tri->texture;
}

bool TriangleObject::DoesRayHitYou(const Ray & ray, RayHitInfo &hitInfo) {
	// Using the CLOCKWISE WINDING routine
	
	// Info for the triangles X values
	double a = this->a.x - this->b.x, b = this->a.x - this->c.x, c = ray.direction.x, d = this->a.x - ray.origin.x;
	// Info for the triangles Y values
	double e = this->a.y - this->b.y, f = this->a.y - this->c.y, g = ray.direction.y, h = this->a.y - ray.origin.y;
	// Info for the triangles Z values
	double i = this->a.z - this->b.z, j = this->a.z - this->c.z, k = ray.direction.z, l = this->a.z - ray.origin.z;

	// Complicataed Triangle mathematics
	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e *j - f * i;

	double inverse_denom = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inverse_denom;

	if(beta < 0.0) return false;

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inverse_denom;
		
	if(gamma < 0.0) return false;
	if(beta + gamma > 1.0) return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inverse_denom;

	if(t < 0.01f) return false;

	hitInfo.t = (float)t;
	hitInfo.point = ray.GetPointAt((float)(t));
	hitInfo.pHitObject = this;
	hitInfo.ray = ray;
	hitInfo.normal = this->normal;
	hitInfo.baryCoord.alpha = 1.0f - beta - gamma;
	hitInfo.baryCoord.beta = beta;
	hitInfo.baryCoord.gamma = gamma;
	hitInfo.tu = hitInfo.baryCoord.Calculate(this->auv.tu, this->buv.tu, this->cuv.tu);
	hitInfo.tv = hitInfo.baryCoord.Calculate(this->auv.tv, this->buv.tv, this->cuv.tv);

	return true;
}

void TriangleObject::CalculateNormal() {
	this->normal = CrossProduct((this->b - this->a), (this->c - this->a));
	this->normal.Normalize();
}

PlaneObject::PlaneObject() {
	
	this->triLeft = TriangleObject(Point(-400.0f, -300.0f, 200.0f, 0.0f), // a			
								   Point(-400.0f, 300.0f, 600.0f, 0.0f), // b						
								   Point(400.0f, -300.0f, 200.0f, 0.0f)); // c
	this->triRight = TriangleObject(this->triLeft.b,  // a
								    Point(this->triLeft.c.x, this->triLeft.b.y, this->triLeft.b.z, 0.0f),  // b
									this->triLeft.c);  // c
	this->triLeft.CalculateNormal();
	this->triRight.CalculateNormal();
	TextureCoord temp = TextureCoord();
	temp = triLeft.auv;
	triLeft.auv = triLeft.buv;
	triLeft.buv = triLeft.cuv;
	triLeft.cuv = temp;
	//*************************************************************
	// CHECK DOT/CROSS PRODUCT FOR VERIFICATION 
	//*************************************************************
	this->normal = this->triLeft.normal;
	this->normal.Normalize();
	this->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	this->triLeft.color = this->color;
	this->triRight.color = this->color;
}

PlaneObject::PlaneObject(Point a, Point b, Point c, Point d) {
	//*************************************************************
	// VERIFY D IS VALID 
	//*************************************************************
	this->triLeft = TriangleObject(a, // a			
								   b,  // b						
								   c); // c
	this->triRight = TriangleObject(b,  // a
								    d,  // b
									c);  // c
	this->triLeft.CalculateNormal();
	this->triRight.CalculateNormal();
	TextureCoord temp = TextureCoord();
	temp = triLeft.auv;
	triLeft.auv = triLeft.buv;
	triLeft.buv = triLeft.cuv;
	triLeft.cuv = temp;
	//*************************************************************
	// CHECK DOT/CROSS PRODUCT FOR VERIFICATION 
	//*************************************************************
	this->normal = this->triLeft.normal;
	this->normal.Normalize();
	this->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	this->triLeft.color = this->color;
	this->triRight.color = this->color;
}

PlaneObject::PlaneObject(TriangleObject tri) {
	this->triLeft = TriangleObject(tri.a, // a			
								   tri.b,  // b						
								   tri.c); // c
	this->triRight = TriangleObject(tri.b,  // a
								    Point(tri.c.x, tri.b.y, tri.b.z, 0.0f),  // b
									tri.c);  // c
	this->triLeft.CalculateNormal();
	this->triRight.CalculateNormal();

	//*************************************************************
	// Adjust the left Triangle's texture coordinates so that its 
	// texture will appear correctly
	//*************************************************************
	TextureCoord temp = TextureCoord();
	temp = triLeft.auv;
	triLeft.auv = triLeft.buv;
	triLeft.buv = triLeft.cuv;
	triLeft.cuv = temp;

	//*************************************************************
	// CHECK DOT/CROSS PRODUCT FOR VERIFICATION 
	//*************************************************************
	this->normal = this->triLeft.normal;
	this->normal.Normalize();
	this->color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	this->triLeft.color = this->color;
	this->triRight.color = this->color;
}

PlaneObject::PlaneObject(PlaneObject* plane) {
	this->triLeft = plane->triLeft;
	this->triRight = plane->triRight;
}


bool PlaneObject::DoesRayHitYou(const Ray &ray, RayHitInfo &hitInfo) {
	if(this->triLeft.DoesRayHitYou(ray, hitInfo)) return true;
	if(this->triRight.DoesRayHitYou(ray, hitInfo)) return true;
	return false;
}

void PlaneObject::SetColor(Color c) {
	this->color = c;
	this->triLeft.color = c;
	this->triRight.color = c;
}

void PlaneObject::SetTexture(char* fileName) {
	this->texture.LoadTexture(fileName);
	this->triLeft.texture.LoadTexture(fileName);
	this->triRight.texture.LoadTexture(fileName);
}