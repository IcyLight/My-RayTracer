#include "Texture.h"

MyColor Texture::GetColor(float u, float v)
{
	RGBQUAD* rgb = &RGBQUAD();
	FreeImage_GetPixelColor(tex, u, v, rgb);
	MyColor color = MyColor(*rgb);
	return color;
}



Texture::Texture(FIBITMAP* _tex)
{
	this->tex = tex;
}

