#pragma once

#include "Transform.h"




struct Ray
{
	vec3 origin;
	vec3 dirction;
	int RecurCount;
	float maxDepth;
	void xfrmRay(MyTransform trans);  //�����߽��б任����Ҫ���������ߴ��ӽ�����ת������������
	Ray(vec3 origin, vec3 dirction, int RecurCount, float maxDepth);
	
};