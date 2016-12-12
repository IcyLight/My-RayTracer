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



class Scene  //Scene�������ж���Ĺ����������㣬���壬���ʣ��ƹ⣬��ͼ��
{
public:
	//��Щ�����ɳ�������
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


	MyColor Raycast(const Ray* ray) const; //���ط������߷��ص����յ���ɫ
	MyColor GetColor(const HitPoint* hit,const Ray* ray, const Geometry* geometry) const; //��ȡ���߷�������������ɫ
	bool visibile(const Ray* ray,const Light* light) const; 
	Ray RayThurPixel(const Camera* cam, int j, int i, float w) const; //���ش�Խ��Ļ�����ص�����
	Scene(float MaxRayDepth, int RecursiveMaxDepth,RenderMode renderMode=RenderMode::VertexNormalMode); 

	RenderMode renderMode; //��Ⱦģʽ
	float MaxRayDepth; //���ߵ���Զ����
	int RecursiveMaxDepth = 1;  //�ڳ����й��߷����������



	//"the law of three"������д���������ˣ��ɴ��ֹ����
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
extern vec3 LightModelConstant ;  //����ģ���й�ǿ�����˥����ʽ����������c1,c2,c3
extern MyTransform LookAtTrans;











//vec3 lookAtTranslate(const vec3 &eye, const vec3 &center, const vec3 &up);





