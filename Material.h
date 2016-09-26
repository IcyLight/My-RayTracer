#pragma once
#include "MyColor.h"
#include "Texture.h"
#include <string>

using namespace std;

struct Matieral
{
	MyColor diffuse;
	MyColor specular;
	MyColor emission;
	MyColor ambient;
	float  shininess;
	string name;
	Texture* map_Ka;
	Texture* map_Kd;
	Texture* map_Ks;
	Matieral(MyColor diffuse, MyColor specular, MyColor emission, MyColor ambient, float shininess,
		Texture* map_Ka=NULL, Texture* map_Kd = NULL, Texture* map_Ks = NULL);

	Matieral(string _name)
	{
		
		this->name = _name;
	}

	Matieral() 
	{
	}

};