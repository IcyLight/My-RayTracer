
#include "Scene.h"

#define RECURSIVE 1


//column major


const float DefaultLightIntensity = 1;

Scene curScene = Scene(100,1);
Camera curCamera = Camera();
stack<MyTransform> TransformStack = stack<MyTransform>();

vec3 LightModelConstant = vec3(1, 0.1, 0.05);  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3
MyTransform LookAtTrans;



Scene::Scene(float MaxRayDepth, int RecursiveMaxDepth)
{
	this->MaxRayDepth = MaxRayDepth;
	this->RecursiveMaxDepth = RecursiveMaxDepth;
}

MyColor Scene::Raycast(Ray ray)
{
	if (ray.RecurCount > RecursiveMaxDepth)
	{
		return MyColor{ 0,0,0,0 };
	}
	
	//vector<HitPoint> SceneHits;
	//vector< vector<Geometry>::iterator > HitGeometry;
	vector<MyHit> SceneHits;
	//注意vector存储时会切掉子类“多余”的部分，导致虚函数失效，参见http://stackoverflow.com/questions/16872584/virtual-functions-and-vector-iterator
	//此处引用vector保存指针
	for (vector<Geometry*>::iterator i = curScene.GeometryArray.begin(); i != curScene.GeometryArray.end(); i++)  
	{
		HitPoints hps = (*i)->Intersect(ray);
		for (int x = 0; x < hps.PointCount; x++)
		{
			HitPoint hp = hps.hps[x];
			MyHit hit = MyHit(hp, *i);
			if (true)
			{
				SceneHits.push_back(hit);
			}

		}

	}

	sort(SceneHits.begin(), SceneHits.end());
	if (SceneHits.empty())
	{
		return MyColor(0, 0, 0, 0);
	}
	MyColor color = GetColor(SceneHits[0].hp, ray,SceneHits[0].obj);

	return color;
}

MyColor Scene::GetColor(HitPoint hit, Ray ray, const Geometry* geometry)  //在视角空间中获取颜色
{
	const MyColor& diffuse = geometry->m.diffuse;
	const MyColor& specular = geometry->m.specular;
	const MyColor& emission = geometry->m.emission;
	const MyColor& ambient = geometry->m.ambient;
	const float&  shininess = geometry->m.shininess;
	vec3& hitpos = hit.position;
	vec3& normal = hit.normal;


	MyColor PointColor = emission + ambient;
	



	for (vector<Light>::iterator i = curScene.LightArray.begin(); i != curScene.LightArray.end(); i++)
	{
		

		

		if (i->type == LightType::Point )
		{

			vec3 lightIn = hitpos - i->pos;
			float distance = vecLength(lightIn);
			lightIn = normalize(lightIn);
			if (visibile(Ray(hitpos + normalize(-lightIn)*0.001f, -lightIn, RecursiveMaxDepth,MaxRayDepth),&(*i)))
			{
				vec3 H = normalize(-ray.dirction - lightIn);
				float disFactor = i->Intensity / (LightModelConstant.x + LightModelConstant.y *distance + LightModelConstant.z *distance *distance);
				//Debug用
				float fgh = disFactor * glm::max(dot(normal, -lightIn), 0.f);
				float Hndot = dot(H, normal);
				float nldot = dot(normal, -lightIn);
				//
				MyColor LightColor = i->color;
				MyColor diffuseColor = LightColor* ( diffuse* disFactor * glm::max(dot(normal, -lightIn), 0.f) );
				MyColor specularColor = LightColor * ( specular* disFactor*pow(glm::max(dot(H, normal), 0.f), shininess) );

				PointColor = PointColor + diffuseColor + specularColor;
				
			}
		}
		else if (i->type == LightType::Dirctional)
		{
			
			vec3 lightIn = -i->pos;
			vec3 H = normalize(-ray.dirction - lightIn);
			//MyColor l1 = diffuse*glm::max(dot(normal, -lightIn), 0.f);
			//MyColor l2 = specular*pow(glm::max(dot(H, normal), 0.f), shininess);
			//MyColor LightColor = diffuse*glm::max(dot(normal, -lightIn), 0.f) + specular*pow(glm::max(dot(H, normal), 0.f), shininess);  //光照模型
			MyColor LightColor = i->color;
			MyColor diffuseColor = LightColor* diffuse *glm::max(dot(normal, -lightIn), 0.f);
			MyColor specularColor = LightColor * specular*pow(glm::max(dot(H, normal), 0.f), shininess);
			
			PointColor = PointColor + diffuseColor + specularColor;
			
		}
		else if (i->type == LightType::ambient)
		{
			
			PointColor = PointColor + i->color;
		}
	}

	if (RECURSIVE)
	{
		vec3 re = reflect(ray.dirction, normal);
		//从模型坐标转回世界坐标
		/*
		MyTransform imt = this->transform.MyInverse();
		ray.xfrmRay(imt);
		imt.AffineTrans(hitpos);
		normal = normal * mat3(imt.trans);
		*/
		//PointColor = Color_Modulate(PointColor, 0.75, Raycast(Ray(hitpos + reflect(ray.dirction, normal)*0.01f, reflect(ray.dirction, normal), ray.depth + 1), this), 0.25);
		PointColor = PointColor + specular*Raycast(Ray(hitpos + re*0.001f, re, ray.RecurCount + 1, MaxRayDepth));
		//注意reflect的参数I是入射方向的反向
	}

	
	return PointColor;
}


bool Scene::visibile(Ray ray,Light* light)
{
	float OLdis = vecLength(light->pos - ray.origin);

	vector<MyHit> SceneHits;
	for (vector<Geometry*>::iterator i = curScene.GeometryArray.begin(); i != curScene.GeometryArray.end(); i++)
	{
		HitPoints hps = (*i)->Intersect(ray);
		for (int x = 0; x < hps.PointCount; x++)
		{
			HitPoint hp = hps.hps[x];
			MyHit hit = MyHit(hp, *i);
			if (vecLength(hp.position - ray.origin) < OLdis)
			{

				SceneHits.push_back(hit);
			}

		}
	}


	if (SceneHits.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}



Ray Scene::RayThurPixel(Camera cam, int j, int i, float w)
{
	//vec3 u = (w*tanf(radians(cam.FOVx/2) )*(-ScreenWidth + 2 * j) / ScreenWidth)*vec3(1, 0, 0);
	//vec3 v = (w*tanf(radians(cam.FOVy/2))*(-ScreenHeight + 2 * i) / ScreenHeight)*vec3(0, 1, 0);
	float sw = (float)cam.ScreenWidth;
	float sh = (float)cam.ScreenHeight;
	float aspect = sw / sh;
	float H = w*tanf(radians(cam.FOVy / 2));
	float W = H*aspect;
	vec3 u = W*(-sw + 2 * j + 1.f) / sw *vec3(1, 0, 0);  //注意是从像素中心射出，所以要加1.0作为补偿
	vec3 v = H*(-sh + 2 * i + 1.f) / sh*vec3(0, 1, 0);
	Ray r = Ray(vec3(0, 0, 0), vec3(0, 0, -1)*w + u + v, 0, MaxRayDepth);
	return r;
}





















