#pragma once

#include "Transform.h"




struct Ray
{
	vec3 origin;
	vec3 dirction;
	int RecurCount;
	float maxDepth;
	void xfrmRay(MyTransform trans);  //�����߽��з���任
	Ray(vec3 origin, vec3 dirction, int RecurCount, float maxDepth);
};