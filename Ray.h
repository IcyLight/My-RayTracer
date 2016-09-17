#pragma once

#include "Transform.h"




struct Ray
{
	vec3 origin;
	vec3 dirction;
	int RecurCount;
	float maxDepth;
	void xfrmRay(MyTransform trans);  //对射线进行仿射变换
	Ray(vec3 origin, vec3 dirction, int RecurCount, float maxDepth);
};