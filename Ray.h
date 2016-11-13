#pragma once

#include "Transform.h"




struct Ray
{
	vec3 origin;
	vec3 dirction;
	int RecurCount;
	float maxDepth;
	void xfrmRay(MyTransform trans);  //对射线进行变换，主要用来把射线从视角坐标转换到物体坐标
	Ray(vec3 origin, vec3 dirction, int RecurCount, float maxDepth);
	
};