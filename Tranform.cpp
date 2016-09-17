#include "Transform.h"



mat4 Rotate(float degrees, vec3 axis)
{

	vec3 ax = glm::normalize(axis);
	float x = ax.x;
	float y = ax.y;
	float z = ax.z;
	float radians = glm::radians(degrees);
	float c = cos(radians);
	float s = sin(radians);
	mat3 I = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	mat3 aStar = mat3(0, z, -y, -z, 0, x, y, -x, 0);
	mat3 n_nt = mat3(x*x, x*y, x*z, x*y, y*y, z*y, x*z, y*z, z*z);
	mat3 t = I*c + n_nt*(1 - c) + aStar*s;
	mat4 r_mt = mat4(t[0][0], t[0][1], t[0][2], 0, t[1][0], t[1][1], t[1][2], 0
		, t[2][0], t[2][1], t[2][2], 0, 0, 0, 0, 1);

	return r_mt;  //旋转矩阵计算公式


}





mat4 Scale(vec3 v)
{
	mat4 m(0);
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
	m[3][3] = 1;
	return m;
}

mat4 Translate(vec3 dv)
{
	mat4 ret = mat4(0);
	ret[3][0] = dv.x;
	ret[3][1] = dv.y;
	ret[3][2] = dv.z;
	ret[0][0] = 1;
	ret[1][1] = 1;
	ret[2][2] = 1;
	ret[3][3] = 1;

	return ret;
}


//glm的LookAt左手系
mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{

	vec3 f = glm::normalize(center - eye);
	vec3 u = glm::normalize(up);
	vec3 s = glm::normalize(glm::cross(f, u));
	u = glm::cross(s, f);

	mat4 Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;

	Result[3][0] = -glm::dot(s, eye);
	Result[3][1] = -glm::dot(u, eye);
	Result[3][2] = glm::dot(f, eye);

	return Result;

}


float vecLength(vec3 v)
{
	float l = sqrtf(dot(v, v));
	return l;

}

double dvecLength(vec3 v)
{
	double I = sqrt(dot(v, v));
	return I;
}


void MyTransform::AffineTrans(vec3& v)
{
	mat4& Mytrans = this->trans;
	vec3 dv = vec3(Mytrans[3][0], Mytrans[3][1], Mytrans[3][2]);
	mat3 mt = mat3(Mytrans[0][0], Mytrans[0][1], Mytrans[0][2], Mytrans[1][0], Mytrans[1][1], Mytrans[1][2], Mytrans[2][0], Mytrans[2][1], Mytrans[2][2]);
	v = mt*v;
	v += dv;
}

void MyTransform::NormalTrans(vec3& v)
{
	mat3 lm = mat3(transpose(inverse(this->trans)));  //向量变换矩阵是原矩阵的逆矩阵的转置
	v = lm*v;
}

MyTransform::MyTransform(mat4 trans)
{
	this->trans = trans;
}
MyTransform::MyTransform()
{
	trans = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}
MyTransform MyTransform::MyInverse()
{
	return MyTransform(inverse(this->trans));
}