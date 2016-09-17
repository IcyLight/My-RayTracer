#pragma once
#include "Transform.h"
#include "Material.h"
#include "Ray.h"
enum GeometryType
{
	sph, tri, infinite
};

struct HitPoint
{
	vec3 position;
	vec3 normal;
	float depth;
	GeometryType type;
	HitPoint() {}
	HitPoint(vec3 pos, vec3 nor, double depth, GeometryType type);
	bool operator< (const HitPoint& h);
	void Trans(MyTransform m);

};

struct HitPoints
{
	HitPoint hps[3];
	int PointCount = 0;
	bool push(HitPoint hp);
	HitPoints()
	{
		PointCount = 0;
	}
};

class Geometry
{
public:



	//MyColor GetColor(vec3 hitpos, vec3 normal, Ray ray);
	virtual HitPoints Intersect(Ray ray)
	{
		return HitPoints();
	}
	Matieral m;
	MyTransform transform;

};




struct MyHit
{
	HitPoint hp;
	Geometry* obj;
	MyHit(HitPoint hp, Geometry* obj)
	{
		this->hp = hp;
		this->obj = obj;
	}
	bool operator< (const MyHit& h)
	{
		return this->hp.depth < h.hp.depth;
	}
};







class Sphere : public Geometry
{
public:
	vec3 pos;
	float radius;

	virtual HitPoints Intersect(Ray ray);
	Sphere(vec3 pos, float radius, Matieral m, MyTransform transform);


};




class Triangle : public Geometry
{
public:
	vec3 a, b, c;
	virtual HitPoints Intersect(Ray ray);
	Triangle(vec3 pA, vec3 pB, vec3 pC, Matieral m, MyTransform transform);


};
