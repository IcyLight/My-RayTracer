#pragma once
#include "MyColor.h"
#include "Ray.h"
#include "Geometry.h"
#include <vector>




enum LightType
{
	Point, Dirctional, ambient
};
struct Light
{
	vec3 pos;
	MyColor color;
	float Intensity;
	LightType type;
	Light(vec3 pos, MyColor color, LightType type, float Intensity = 1);

};
