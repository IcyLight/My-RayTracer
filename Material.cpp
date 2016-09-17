#include "Material.h"



Matieral::Matieral(MyColor diffuse, MyColor specular, MyColor emission, MyColor ambient, float shininess)
{
	this->diffuse = diffuse;
	this->specular = specular;
	this->emission = emission;
	this->ambient = ambient;
	this->shininess = shininess;

}