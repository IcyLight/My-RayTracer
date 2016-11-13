#pragma once
#include "MyColor.h"
#include "Texture.h"
#include "Transform.h"
#include <string>

using namespace std;

enum MapType
{
	ambientMap,diffuseMap,specularMap,normalMap
};

class Matieral
{
public:
	MyColor diffuse;
	MyColor specular;
	MyColor emission;
	MyColor ambient;
	float  shininess;
	string name;
	Texture* map_Ka;
	Texture* map_Kd;
	Texture* map_Ks;
	Texture* map_Kn;
	Texture* map_bump;

	Matieral(MyColor diffuse, MyColor specular, MyColor emission, MyColor ambient, float shininess,
		Texture* map_Ka=nullptr, Texture* map_Kd = nullptr, Texture* map_Ks = nullptr,Texture* map_Kn = nullptr,Texture* map_bump=nullptr);

	MyColor GetMapColor(MapType type,vec3 uvw);
	vec3 GetMapVec(MapType type, vec3 uvw);


	Matieral(string _name)
	{
		
		this->name = _name;
	}

	Matieral() 
	{
		MyColor White = MyColor(0, 0, 0, 0);
		this->diffuse = White;
		this->specular = White;
		this->emission = White;
		this->ambient = White;
		this->shininess = 20;
		this->map_Ka = nullptr;
		this->map_Kd = nullptr;
		this->map_Ks = nullptr;
	}

};