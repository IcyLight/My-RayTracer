#include "Light.h"

using namespace std;

Light::Light(vec3 pos, MyColor color,  LightType type, float Intensity)
{
	this->pos = pos;
	this->color = color;
	this->type = type;
	this->Intensity = Intensity;
}


