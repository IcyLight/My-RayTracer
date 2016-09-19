#pragma once

#include "lib\glm\glm\glm.hpp"


#include <vector>
#include <stack>
#include <memory>
#include <algorithm>
#include "lib\FreeImage.h"
#include "Transform.h"
#include "MyColor.h"
#include "Material.h"
#include "Light.h"
#include "Ray.h"
#include "Camera.h"
using namespace glm;
using namespace std;




















class Scene
{
public:
	//vector<Sphere> sphArray;
	//vector<Triangle> triArray;
	vector<Geometry*> GeometryArray;
	vector<Light> LightArray;
	vector<Vertex> vertexArray;
	vector<Matieral*> MatieralArray;
	Matieral defaultMatieral;

	vector<vec3> vPosArray;
	vector<vec3> vNorArray;
	vector<vec3> vUVArray;

	
	MyColor Raycast(Ray ray);
	MyColor GetColor(HitPoint hit,  Ray ray, const Geometry* geometry);
	bool visibile(Ray ray,Light* light);
	Ray RayThurPixel(Camera cam, int j, int i, float w);
	Scene(float MaxRayDepth, int RecursiveMaxDepth);

	float MaxRayDepth; 
	int RecursiveMaxDepth = 1;
};



MyColor Raycast(Ray ray);



extern Scene curScene ;
extern Camera curCamera ;
extern stack<MyTransform> TransformStack ;
extern vec3 LightModelConstant ;  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3
extern MyTransform LookAtTrans;











//vec3 lookAtTranslate(const vec3 &eye, const vec3 &center, const vec3 &up);





