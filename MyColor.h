#pragma once

#include "FreeImage.h"
#include "lib\glm\glm\glm.hpp"

struct MyColor
{
	
	//RGBQUAD color;
	float R, G, B, A;
	MyColor(float R, float G, float B, float A);
	MyColor(RGBQUAD rgb);
	MyColor();
	MyColor operator*(const float& d) const;
	MyColor operator+(const MyColor& mc) const;
	MyColor operator*(const MyColor& mc) const;

	operator glm::vec3(); //由法线图的颜色转换为法线

	RGBQUAD GetRBGQUAD();

};
