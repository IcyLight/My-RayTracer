#pragma once
#include "MyColor.h"
#include "FreeImage.h"
#include "Transform.h"

class Texture
{
public:
	MyColor GetColor(vec3 uvw);
	int height, width;
	
	MyColor GetColor(float u, float v);
	Texture(FIBITMAP* tex,int width,int height);


private:
	FIBITMAP* tex;



};

