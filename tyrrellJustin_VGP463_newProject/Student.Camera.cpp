// Student.Ray.cpp
///////////////////////////////////////////////////////////////////////////////////////////////

#include "Student.Camera.h"

void Orientation::Normalize(){
	this->forward.Normalize();
	this->right.Normalize();
	this->up.Normalize();
}

Ray Camera::GetRay(float x, float y) {
	Ray ray;
	
	ray.origin.x = x;
	ray.origin.y = y;
	ray.origin.z = 0.0f;
	ray.origin.w = 1.0f;
	ray.direction.x = 0.0f;
	ray.direction.y = 0.0f;
	ray.direction.z = 1.0f;
	ray.direction.w = 0.0f;
	
	return ray;
}

void Camera::Initialize() {
	this->position.x = 0.0;
	this->position.y = 0.0;
	this->position.z = 0.0;
	this->position.z = 0.0;
	this->orientation.forward.x = 0.0;
	this->orientation.forward.y = 0.0;
	this->orientation.forward.z = 1.0;
	this->orientation.right.x = 1.0;
	this->orientation.right.y = 0.0;
	this->orientation.right.z = 0.0;
	this->orientation.up.x = 0.0;
	this->orientation.up.y = 1.0;
	this->orientation.up.z = 0.0;
	this->direction.x = 0.0;
	this->direction.y = 0.0;
	this->direction.z = 1.0;
	this->ray.direction.x = 0.0;
	this->ray.direction.y = 0.0;
	this->ray.direction.z = 1.0;
	this->ray.origin.x = 0.0;
	this->ray.origin.y = 0.0;
	this->ray.origin.z = 0.0;
}

Ray HemisphericalCamera::GetRay(float x, float y) {
	x = (x / this->image_plane_width) * 4.0f;
	y = (y / this->image_plane_height) * 4.0f;
	float z = 1.0f;
	Point tip = x * orientation.right + y * orientation.up + z * orientation.forward;
	Vector direction = tip;
	direction.Normalize();
	Ray result;
	result.direction = direction;
	result.origin = this->position;
	return result;
}

void FixedOrthographicCamera::Initialize() {
	Camera::Initialize();
}

Ray FixedOrthographicCamera::GetRay(float x, float y) {
	Ray ray;
	
	ray.origin.x = x;
	ray.origin.y = y;
	ray.origin.z = 0.0;
	ray.origin.w = 1.0;
	ray.direction.x = 0.0;
	ray.direction.y = 0.0;
	ray.direction.z = 1.0;
	ray.direction.w = 0.0;
	
	return ray;
}

void FixedPerspectiveCamera::Initialize() {
	Camera::Initialize();

	this->focalPoint = this->position;
	this->focalLength = _FOCAL_LENGTH_DEFAULT;
	this->viewportCenter = this->focalPoint + this->focalLength * orientation.forward;
}

Ray FixedPerspectiveCamera::GetRay(float x, float y) {
	Ray ray;

	ray.origin = this->focalPoint;						// Set the ray's origin to the focal point of the camera
	ray.direction = (this->viewportCenter +				// Get the vector from my focalPoint to my x,y position on the viewport
					 (x * this->orientation.right) + 
					 (y * this->orientation.up)) - 
					 this->focalPoint;
	ray.direction.Normalize();
	return ray;
}

void FixedPerspectiveCamera::LookAt(double x, double y, double z) {
	this->orientation.forward = Point((float) x,(float) y,(float) z, 0.0f) - this->position;
	this->orientation.right = CrossProduct(Vector(0.0f, 1.0f, 0.0f, 1.0f), this->orientation.forward);
	this->orientation.up = CrossProduct(this->orientation.forward, this->orientation.right);
	
	//this->orientation.forward = Point((float) x,(float) y,(float) z, 0.0f) + this->position;
	//this->orientation.right = CrossProduct(this->orientation.forward, Vector(0.0f, 1.0f, 0.0f, 0.0f));
	//this->orientation.up = CrossProduct(this->orientation.right, this->orientation.forward);

	this->orientation.Normalize();
}

void FixedPerspectiveCamera::SetFocalPoint(float f) {

}