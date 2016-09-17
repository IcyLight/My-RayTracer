#pragma once
#include "Transform.h"

class Camera
{
public:
	float FOVx, FOVy, near;
	vec3 lookFrom;
	vec3 lookAt;
	vec3 up;

	int ScreenWidth = 128;
	int ScreenHeight = 128;


	Camera(float FOVx, float FOVy, float near, vec3 lookFrom, vec3 lookAt, vec3 up, float ScreenWidth = 2048, float ScreenHeight = 1536);
	Camera() {};
};

