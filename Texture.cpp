#include "Texture.h"
#include <stdio.h>
#include "Debug.h"

MyColor Texture::GetColor(float u, float v)
{

	RGBQUAD* rgb = &RGBQUAD();
	FreeImage_GetPixelColor(tex, u*width, v*height, rgb); //这里需要图片宽高，暂时没做

	

	MyColor color = MyColor(*rgb);

	return color;
}
MyColor Texture::GetColor(vec3 uvw)
{
	return GetColor(uvw.x, uvw.y);
}


Texture::Texture(FIBITMAP* tex, int width, int height)
{
	this->tex = tex;
	this->width = width;
	this->height = height;

}

