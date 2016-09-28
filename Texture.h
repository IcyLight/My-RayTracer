#pragma once
#include "MyColor.h"
#include "FreeImage.h"
#include "Transform.h"

class Texture
{
public:
	MyColor GetColor(vec3 uvw);
	
	MyColor GetColor(float u, float v);
	Texture(FIBITMAP* _tex);


private:
	FIBITMAP* tex;



};

