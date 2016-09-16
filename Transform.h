#pragma once
#include "lib\glm\glm\glm.hpp"

using namespace glm;

mat4 Rotate(float degrees, vec3 axis);

mat4 Scale(vec3 v);

mat4 Translate(vec3 dv);

mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);



float vecLength(vec3 v);

double dvecLength(vec3 v);

struct MyTransform
{
	mat4 trans;
	//对物体进行仿射变换
	void AffineTrans(vec3& v);
	void NormalTrans(vec3& v);
	MyTransform(mat4 trans)
	{
		this->trans = trans;
	}
	MyTransform()
	{
		trans = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}
	MyTransform MyInverse()
	{
		return MyTransform(inverse(this->trans));
	}

};
