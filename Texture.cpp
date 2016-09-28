#include "Texture.h"
#include <stdio.h>

MyColor Texture::GetColor(float u, float v)
{

	RGBQUAD* rgb = &RGBQUAD();
	FreeImage_GetPixelColor(tex, u*255, v, rgb); //这里需要图片宽高，暂时没做

	

	MyColor color = MyColor(*rgb);
	/*Debug
	printf("uv %f %f\n", u * 255, v);
	printf("rgb %d %d %d\n", rgb->rgbRed, rgb->rgbGreen, rgb->rgbBlue);
	printf("color %f %f %f\n\n", color.R, color.G, color.B);
	*/

	return color;
}
MyColor Texture::GetColor(vec3 uvw)
{
	return GetColor(uvw.x, uvw.y);
}


Texture::Texture(FIBITMAP* _tex)
{
	this->tex = _tex;
}

