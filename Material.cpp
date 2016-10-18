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


MyColor Matieral::GetMapColor(MapType type, vec3 uvw)
{
	
	switch (type)
	{
	case ambientMap:
		if (this->map_Ka != nullptr)
		{
			 return map_Ka->GetColor(uvw)*this->ambient;
		}
		else
		{
			return ambient;
		}
		break;
	case diffuseMap:
		if (this->map_Kd != nullptr)
		{
			return map_Kd->GetColor(uvw)*this->diffuse;
		}
		else
		{
			return diffuse;
		}
		break;
	case specularMap:
		if (this->map_Ks != nullptr)
		{
			return map_Ks->GetColor(uvw)*this->specular;
		}
		else
		{
			return specular;
		}
		break;
	case normalMap:
		return MyColor();
		break;
	default:
		break;
	}
}