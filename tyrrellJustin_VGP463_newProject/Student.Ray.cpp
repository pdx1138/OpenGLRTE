// Student.Ray.cpp
///////////////////////////////////////////////////////////////////////////////////////////////

#include "Student.Ray.h"

RenderRegion::RenderRegion() {
	this->x1 = 0.0f;
	this->x2 = 0.0f;
	this->y1 = 0.0f;
	this->y2 = 0.0f;
	this->complete = false;
	this->active = false;
}

RenderRegion::RenderRegion(float x1, float y1, float x2, float y2) {
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->complete = false;
	this->active = false;
}


Ray::Ray() {

}

RayHitInfo::RayHitInfo() {
	this->pixel = 0x00000000;
	this->t = 0;
	this->tu = 0;
	this->tv = 0;
	this->normal = Vector();
	this->tangent = Vector();
	this->reflection = Vector();
	this->refraction = Vector();
	this->baryCoord = BaryCoord();
	this->pHitObject = nullptr;
	this->ray = Ray();	
	this->point = Point();
	
}

Point Ray::GetPointAt(float t) const {
	Point result;

	result.x = origin.x + t * direction.x;
	result.y = origin.y + t * direction.y;
	result.z = origin.z + t * direction.z;
	result.w = origin.w + t * direction.w;

	return result;
}

float BaryCoord::Calculate(float a, float b, float c) const{
	return this->alpha * a + this->beta * b + this->gamma * c;
}

bool GetNearestHit(const Ray &ray, Object &obj, RayHitInfo &hitInfo) {
	RayHitInfo nearestHit;
	nearestHit.t = 99999.0f;
	bool bHit = false;

	RayHitInfo h;
	if(obj.DoesRayHitYou(ray, h)) {
		if(h.t < nearestHit.t) {
			nearestHit = h;
			bHit = true;
		}
	}
	hitInfo = nearestHit;
	return bHit;
}

bool GetNearestHit(const Ray &ray, List<Object*> &objList, RayHitInfo &hitInfo) {
	RayHitInfo nearestHit;
	nearestHit.t = 99999.0f;
	bool bHit = false;

	for(int n = 0; n < objList.length; ++n) {
		Object* obj = objList[n];
		RayHitInfo info;
		if(obj->DoesRayHitYou(ray, info)) {
			if(info.t < nearestHit.t) {
				nearestHit = info;
				bHit = true;
			}
		}
	}
	hitInfo = nearestHit;
	return bHit;
}

bool GetNearestHit(const Ray &ray, Scene* scene, RayHitInfo &hitInfo) {
	RayHitInfo nearestHit;
	nearestHit.t = 99999.0;
	bool bHit = false;
	for(int n = 0; n < scene->objects.length; n++) {
		Object* obj = scene->objects[n];
		RayHitInfo info;
		if(obj->DoesRayHitYou(ray, info)) {
			if(info.t < nearestHit.t) {
				nearestHit = info;
				bHit = true;
			}
		}
	}
	hitInfo = nearestHit;
	return bHit;
}

bool GetNearestSphereHit(const Ray &ray, SphereObject* obj, RayHitInfo &hitInfo) {
	RayHitInfo nearestHit;
	nearestHit.t = 99999.0f;
	bool bHit = false;

	RayHitInfo h;
	if(obj->DoesRayHitYou(ray, h)) {
		if(h.t < nearestHit.t) {
			nearestHit = h;
			bHit = true;
		}
	}
	hitInfo = nearestHit;
	return bHit;
}

bool GetNearestSphereHit(const Ray &ray, List<Object*> &objList, RayHitInfo &hitInfo) {
	RayHitInfo nearestHit;
	nearestHit.t = 99999.0f;
	bool bHit = false;

	for(int n = 0; n < objList.length; ++n) {
		Object* obj = objList[n];
		RayHitInfo info;
		if(obj->DoesRayHitYou(ray, info)) {
			if(info.t < nearestHit.t) {
				nearestHit = info;
				bHit = true;
			}
		}

	}
	hitInfo = nearestHit;
	return bHit;
}

