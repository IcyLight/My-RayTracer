#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <algorithm>
#include <thread>

#include "lib\glm\glm\glm.hpp"
#include "lib\FreeImage.h"

#include "Transform.h"
#include "MyColor.h"
#include "Material.h"
#include "Light.h"
#include "Ray.h"
#include "Camera.h"
using namespace glm;
using namespace std;


enum RenderMode
{
	VertexNormalMode, NormalMapMode
};



class Scene
{
public:
	//vector<Sphere> sphArray;
	//vector<Triangle> triArray;
	vector<Geometry*> GeometryArray;
	vector<Light*> LightArray;
	vector<Vertex*> vertexArray;
	vector<Matieral*> MatieralArray;
	//Matieral defaultMatieral;

	vector<vec3> vPosArray;
	vector<vec3> vNorArray;
	vector<vec3> vUVArray;


	MyColor Raycast(const Ray* ray) const;
	MyColor GetColor(const HitPoint* hit,const Ray* ray, const Geometry* geometry) const;
	bool visibile(const Ray* ray,const Light* light) const;
	Ray RayThurPixel(const Camera* cam, int j, int i, float w) const;
	Scene(float MaxRayDepth, int RecursiveMaxDepth,RenderMode renderMode=RenderMode::VertexNormalMode);

	RenderMode renderMode;
	float MaxRayDepth; 
	int RecursiveMaxDepth = 1;
};

FIBITMAP* Display(Camera cam,Scene* scene);






extern Camera HWCamera ;
extern stack<MyTransform> TransformStack ;
extern vec3 LightModelConstant ;  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3
extern MyTransform LookAtTrans;











//vec3 lookAtTranslate(const vec3 &eye, const vec3 &center, const vec3 &up);





