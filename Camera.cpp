#include "Camera.h"


Camera::Camera(float FOVx, float FOVy, float near, vec3 lookFrom, vec3 lookAt, vec3 up, float ScreenWidth, float ScreenHeight)
{
	this->FOVx = FOVx;
	this->FOVy = FOVy;
	this->near = near;
	this->lookFrom = lookFrom;
	this->lookAt = lookAt;
	this->up = up;
	this->ScreenWidth = ScreenWidth;
	this->ScreenHeight = ScreenHeight;
}