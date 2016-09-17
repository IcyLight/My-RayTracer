#pragma once

#include "lib\glm\glm\glm.hpp"


#include <vector>
#include <stack>
#include <memory>
#include <algorithm>
#include "lib\FreeImage.h"
#include "Transform.h"
#include "MyColor.h"

using namespace glm;
using namespace std;






struct Ray
{
	vec3 origin;
	vec3 dirction;
	int depth;
	void xfrmRay(MyTransform trans);  //对射线进行仿射变换
	Ray(vec3 origin, vec3 dirction,int depth)
	{
		this->origin = origin;
		this->dirction = normalize(dirction);
		this->depth = depth;
	}
};

struct Matieral
{
	MyColor diffuse;
	MyColor specular;
	MyColor emission;
	MyColor ambient;
	float  shininess;
	Matieral( MyColor diffuse, MyColor specular, MyColor emission, MyColor ambient, float shininess)
	{
		this->diffuse = diffuse;
		this->specular = specular;
		this->emission = emission;
		this->ambient = ambient;
		this->shininess = shininess;
		
	}

	Matieral(){}

};


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
	HitPoint()
	{

	}
	HitPoint(vec3 pos, vec3 nor, double depth, GeometryType type)
	{
		position = pos;
		normal = normalize(nor);
		this->depth = depth;
		this->type = type;
		//this->matieral = matieral;
	}
	bool operator< (const HitPoint& h)
	{
		return this->depth < h.depth;
	}
	void Trans(MyTransform m)
	{
		m.AffineTrans(this->position);
		mat3 linear = mat3(m.trans);
		this->normal = linear*this->normal;
	}

};

struct HitPoints 
{
	HitPoint hps[3];
	int PointCount = 0;
	bool push(HitPoint hp)
	{
		if (PointCount + 1 > 2)
		{
			return false;
		}
		else
		{
			hps[PointCount] = hp;
			PointCount++;
		}
	}
	HitPoints()
	{
		PointCount = 0;
	}
};


class Geometry
{
public:



	MyColor GetColor(vec3 hitpos, vec3 normal, Ray ray);
	virtual HitPoints Intersect(Ray ray) 
	{
		return HitPoints();
	}
	Matieral m;
	MyTransform transform;
	
};

enum LightType
{
	Point,Dirctional, ambient
};
struct Light
{
	vec3 pos;
	MyColor color;
	float Intensity;
	bool visibile(Ray ray);
	LightType type;
	Light(vec3 pos, MyColor color, float Intensity,LightType type)
	{
		this->pos = pos;
		this->color = color;
		this->Intensity = Intensity;
		this->type = type;
	}

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
	//sphHitPoints Sph_Intersect(Ray ray);
	
	virtual HitPoints Intersect(Ray ray);
	Sphere(vec3 pos,float radius, Matieral m,MyTransform transform)
	{
		this->pos = pos;
		this->radius = radius;
		this->m = m;
		this->transform = transform;
		
	}

	
};




class Triangle : public Geometry
{
public:
	//HitPoint tri_Intersect(Ray ray);
	vec3 a, b, c;
	virtual HitPoints Intersect(Ray ray);
	Triangle(vec3 pA,vec3 pB,vec3 pC,Matieral m,MyTransform transform)
	{
		this->a = pA;
		this->b = pB;
		this->c = pC;
		this->m = m;
		this->transform = transform;
	}
	
	
};

//摄像机在视角空间朝向-z轴
class Camera
{
public:
	float FOVx,FOVy,near;
	vec3 lookFrom;
	vec3 lookAt;
	vec3 up;
	Camera(float FOVx, float FOVy, float near,vec3 lookFrom,vec3 lookAt,vec3 up)
	{
		this->FOVx = FOVx;
		this->FOVy = FOVy;
		this->near = near;
		this->lookFrom = lookFrom;
		this->lookAt = lookAt;
		this->up = up;
	}
	Camera()
	{

	}
};




class Scene
{
public:
	//vector<Sphere> sphArray;
	//vector<Triangle> triArray;
	vector<Geometry*> GeometryArray;
	vector<Light> LightArray;
	vector<vec3> vertexArray;
	vector<Matieral> MatieralArray;
	Matieral defaultMatieral;
};



MyColor Raycast(Ray ray);


extern int ScreenWidth;
extern int ScreenHeight;
extern const float DefaultLightIntensity ;
extern double maxDepth ;
extern int RecursiveMaxDepth ;
extern Scene curScene ;
extern Camera curCamera ;
extern stack<MyTransform> TransformStack ;
extern vec3 LightModelConstant ;  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3
extern MyTransform LookAtTrans;











//vec3 lookAtTranslate(const vec3 &eye, const vec3 &center, const vec3 &up);





