#pragma once
#include "Transform.h"

//�����Ĭ�����ӽǿռ䳯��-z�ᣬλ��ԭ��
class Camera
{
public:
	float FOVx, FOVy, near;
	vec3 lookFrom;
	vec3 lookAt;
	vec3 up;

	int ScreenWidth;
	int ScreenHeight;


	Camera(float FOVx, float FOVy, float near, vec3 lookFrom, vec3 lookAt, vec3 up, float ScreenWidth = 800, float ScreenHeight = 600);
	Camera() {};
};

