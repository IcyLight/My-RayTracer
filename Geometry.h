#pragma once
#include <list>
#include <xtree>
#include "Transform.h"
#include "Material.h"
#include "Ray.h"
#include "Vertex.h"

#include "BSP.h"



enum GeometryType
{
	sph, tri, infinite
};

struct HitPoint
{
	
	vec3 position;
	vec3 normal;
	vec3 uvw;
	float depth;
	GeometryType type;
	HitPoint() {}
	HitPoint(vec3 pos, vec3 nor,vec3 uvw, double depth, GeometryType type);
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
	virtual HitPoints Intersect(const Ray* ray)
	{
		return HitPoints();
	}
	virtual MyTransform GetT2WMatrix(const vec3& uvw) const //获取改点从tangent space转到 world space
	{
		return MyTransform();
	}
	virtual MyTransform GetT2WMatrix(const vec3& uvw, const vec3& pos, const vec3& normal) const
	{
		return MyTransform();
	}
	Matieral* m;
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

	virtual HitPoints Intersect(const Ray* ray);
	
	Sphere(vec3 pos, float radius, Matieral* m, MyTransform transform);


};




class Triangle : public Geometry
{
public:
	Vertex* a, * b, *c;
	vec3 faceNormal;
	virtual HitPoints Intersect(const Ray* ray) const;
	 bool PlaneIntersect(const Ray* line,  vec3* HitPoint);  //求与三角形所在平面的交点

	virtual MyTransform GetT2WMatrix(const vec3& uvw) const;
	virtual MyTransform GetT2WMatrix(const vec3& uvw, const vec3& pos, const vec3& normal) const;
	Triangle(Vertex* _vA, Vertex* _vB, Vertex* _vC, Matieral* m, MyTransform transform);

	float PlaneDistance2Point(const vec3* v) const;

	static BSP_Case GetBSPRelation(const Triangle* object, const Triangle* hyperplane);
	static bool SplitTriangle(const Triangle* hyperPlane,const Triangle* Object ,vector<Triangle*>* Out_Objects,vector<Vertex*>* Out_Vertexs);
};
typedef  BSPNode<Triangle, Vertex,Triangle::GetBSPRelation, Triangle::SplitTriangle> TriangleBSPNode;
typedef BSPTree<Triangle, Vertex, Triangle::GetBSPRelation, Triangle::SplitTriangle> TriangleBSPTree;

void GetBSPClip(const Ray* ray, const TriangleBSPNode* node, list<Triangle*>* tlist);



