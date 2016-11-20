#pragma once
#include "Transform.h"

class Vertex
{
public:
	vec3 pos;
	vec3 normal;
	vec3 uvw;



	Vertex(vec3 _pos)
	{
		this->pos = _pos;
	}
	Vertex(vec3 _pos,vec3 _normal)
	{
		this->pos = _pos;
		this->normal = _normal;
	}
	Vertex(vec3 _pos,vec3 uvw ,vec3 _normal)
	{
		this->pos = _pos;
		this->uvw = uvw;
		this->normal = _normal;
	}
	Vertex()
	{

	}
};


