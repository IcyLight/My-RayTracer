#pragma once
#include "MyColor.h"

struct Matieral
{
	MyColor diffuse;
	MyColor specular;
	MyColor emission;
	MyColor ambient;
	float  shininess;
	Matieral(MyColor diffuse, MyColor specular, MyColor emission, MyColor ambient, float shininess);

	Matieral() 
	{
	}

};