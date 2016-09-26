#pragma once
#include "MyColor.h"
#include "FreeImage.h"

class Texture
{
public:
	MyColor GetColor(float u, float v, float w);
	MyColor GetColor(float u, float v);
	Texture(FIBITMAP* _tex);


private:
	FIBITMAP* tex;



};

