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



class Scene  //Scene负责所有对象的管理，包括顶点，物体，材质，灯光，贴图。
{
public:
	//这些对象由场景管理
	vector<Geometry*> GeometryArray;
	vector<Light*> LightArray;
	vector<Vertex*> vertexArray;
	vector<Matieral*> MatieralArray;
	vector<Texture*> textureArray;
	TriangleBSPTree* bsptree;

	vector<Triangle*> TriangleArray;
	vector<Sphere*> SphereArray;


	

	//Matieral defaultMatieral;

	vector<vec3> vPosArray;
	vector<vec3> vNorArray;
	vector<vec3> vUVArray;


	MyColor Raycast(const Ray* ray) const; //返回发出射线返回的最终的颜色
	MyColor GetColor(const HitPoint* hit,const Ray* ray, const Geometry* geometry) const; //获取射线方向物体表面的颜色
	bool visibile(const Ray* ray,const Light* light) const; 
	Ray RayThurPixel(const Camera* cam, int j, int i, float w) const; //返回穿越屏幕中像素的射线
	Scene(float MaxRayDepth, int RecursiveMaxDepth,RenderMode renderMode=RenderMode::VertexNormalMode); 

	RenderMode renderMode; //渲染模式
	float MaxRayDepth; //光线的最远距离
	int RecursiveMaxDepth = 1;  //在场景中光线反射的最大次数



	//"the law of three"，懒得写另外两个了，干脆禁止复制
	~Scene()
	{
		
		delete bsptree;
		for (int i = 0; i < GeometryArray.size(); ++i)
		{
			delete GeometryArray[i];
		}
		for (int i = 0; i < LightArray.size(); ++i)
		{
			delete LightArray[i];
		}
		for (int i = 0; i < vertexArray.size(); ++i)
		{
			delete vertexArray[i];
		}
		for (int i = 0; i < MatieralArray.size(); ++i)
		{
			delete MatieralArray[i];
		}
		for (int i = 0; i < textureArray.size(); ++i)
		{
			delete textureArray[i];
		}
	}
private:
	Scene(const Scene& that)
	{

	}
	Scene& operator=(const Scene& that)
	{

	}
};

FIBITMAP* Display(Camera cam,Scene* scene);






extern Camera HWCamera ;
extern stack<MyTransform> TransformStack ;
extern vec3 LightModelConstant ;  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3
extern MyTransform LookAtTrans;











//vec3 lookAtTranslate(const vec3 &eye, const vec3 &center, const vec3 &up);





