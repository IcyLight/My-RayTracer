#include "Material.h"



Matieral::Matieral(MyColor diffuse, MyColor specular, MyColor emission, MyColor ambient, float shininess,
	Texture* map_Ka , Texture* map_Kd , Texture* map_Ks )
{
	this->diffuse = diffuse;
	this->specular = specular;
	this->emission = emission;
	this->ambient = ambient;
	this->shininess = shininess;
	this->map_Ka = map_Ka;
	this->map_Kd = map_Kd;
	this->map_Ks = map_Ks;

}