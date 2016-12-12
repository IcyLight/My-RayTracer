
#include "Scene.h"

#include "Debug.h"


//column major


const float DefaultLightIntensity = 1;


Camera HWCamera = Camera();
stack<MyTransform> TransformStack = stack<MyTransform>();

vec3 LightModelConstant = vec3(1, 0, 0);  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3
MyTransform LookAtTrans;



Scene::Scene(float MaxRayDepth, int RecursiveMaxDepth,RenderMode renderMode)
{
	this->MaxRayDepth = MaxRayDepth;
	this->RecursiveMaxDepth = RecursiveMaxDepth;
	this->renderMode = renderMode;
}

MyColor Scene::Raycast(const Ray* ray) const
{
	if (ray->RecurCount > RecursiveMaxDepth)
	{
		return MyColor{ 0,0,0,0 };
	}


	vector<MyHit> SceneHits;
	if (bsptree != nullptr)
	{
		list<Triangle*> clipTriangleList;
		GetBSPClip(ray, bsptree->root,&clipTriangleList);
		//printf("此射线检测三角形个数%d\n", clipTriangleList.size());
		for (list<Triangle*>::const_iterator i = clipTriangleList.cbegin(); i != clipTriangleList.cend(); i++)
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

		for (vector<Sphere*>::const_iterator i = SphereArray.cbegin(); i != SphereArray.cend(); i++)
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
	}
	else
	{
		for (vector<Geometry*>::const_iterator i = GeometryArray.cbegin(); i != GeometryArray.end(); i++)
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
	}
	int i = SceneHits.size();
	sort(SceneHits.begin(), SceneHits.end());
	if (SceneHits.empty())
	{
		return MyColor(0, 0, 0, 0);
	}
	MyColor color = GetColor(&SceneHits[0].hp, ray,SceneHits[0].obj);

	return color;
}

MyColor Scene::GetColor(const HitPoint* hit, const Ray* ray, const Geometry* geometry) const  //在视角空间中获取颜色
{
	
	MyColor ambient = geometry->m->GetMapColor(MapType::ambientMap, hit->uvw);
	MyColor diffuse = geometry->m->GetMapColor(MapType::diffuseMap, hit->uvw);
	MyColor specular = geometry->m->GetMapColor(MapType::specularMap, hit->uvw);

	const MyColor& emission = geometry->m->emission;
	const float&  shininess = geometry->m->shininess;
	const vec3& hitpos = hit->position;

	vec3 normal;
	if (renderMode == RenderMode::VertexNormalMode)
	{
		normal = hit->normal;
	}
	else if (renderMode == RenderMode::NormalMapMode)
	{

		normal = (vec3)geometry->m->GetMapColor(MapType::normalMap, hit->uvw);
		if (normal.z > 0.5f)  //贴图返回的是有效的法线
		{
			MyTransform invr = geometry->transform.MyInverse();

			vec3 Hitnormal = hit->normal;
			vec3 Mpos = hit->position;

			invr.AffineTrans(Mpos);
			invr.NormalTrans(Hitnormal);

			MyTransform T2WTrans = geometry->GetT2WMatrix(hit->uvw, Mpos, normal);

			MyTransform T2VTransfrom = geometry->transform*T2WTrans;


			T2VTransfrom.NormalTrans(normal);
		}
		else
		{
			normal = hit->normal;
		}
	}
	


	MyColor PointColor = emission + ambient;
	



	for (vector<Light*>::const_iterator i = LightArray.cbegin(); i != LightArray.cend(); i++)
	{
		
		const Light* light = *i;

		if (light->type == LightType::Point )
		{

			vec3 lightIn = hitpos - light->pos;
			float distance = vecLength(lightIn);
			lightIn = normalize(lightIn);
			if (visibile(&Ray(hitpos + normalize(-lightIn)*RayOffset, -lightIn, RecursiveMaxDepth,MaxRayDepth),light))
			{
				vec3 H = normalize(-ray->dirction - lightIn);
				float disFactor = light->Intensity / (LightModelConstant.x + LightModelConstant.y *distance + LightModelConstant.z *distance *distance);

				MyColor LightColor = light->color;
				MyColor diffuseColor = LightColor* ( diffuse* disFactor * glm::max(dot(normal, -lightIn), 0.f) );
				MyColor specularColor = LightColor * ( specular* disFactor*pow(glm::max(dot(H, normal), 0.f), shininess) );

				PointColor = PointColor + diffuseColor + specularColor;
				
			}
		}
		else if (light->type == LightType::Dirctional)
		{
			
			vec3 lightIn = normalize(light->pos);  
			vec3 H = normalize(-ray->dirction - lightIn);
			MyColor LightColor = light->color;
			MyColor diffuseColor = LightColor* diffuse *glm::max(dot(normal, -lightIn), 0.f);
			MyColor specularColor = LightColor * specular*pow(glm::max(dot(H, normal), 0.f), shininess);
			

			PointColor = PointColor + diffuseColor + specularColor;
			
			
		}
		else if (light->type == LightType::ambient)
		{
			
			PointColor = PointColor + light->color;
		}
	}

	if (RECURSIVE)
	{
		vec3 re = reflect(ray->dirction, normal);
		PointColor = PointColor + specular*Raycast(&Ray(hitpos + re*RayOffset, re, ray->RecurCount + 1, MaxRayDepth));
		//注意reflect的参数I是入射方向的反向v
	}
	
	return PointColor;
}

void RenderRow(Scene* scene, Camera cam, FIBITMAP* outImage, int rowIndex)
{
	for (int j = 0; j < cam.ScreenWidth; j++)
	{
		Ray r = scene->RayThurPixel(&cam, j, rowIndex, 1);
		MyColor color = scene->Raycast(&r);

		//DEBUG_IN_PIXEL(j, rowIndex, 28, 28,DEBUG_PRINT_COLOR(color) )

		FreeImage_SetPixelColor(outImage, j, rowIndex, &(color.GetRBGQUAD()));
	}
}

FIBITMAP* Display(Camera cam,Scene* scene)
{
	FIBITMAP* m = FreeImage_Allocate(cam.ScreenWidth, cam.ScreenHeight, 24);


	mat4 LookAtMat = lookAt(cam.lookFrom, cam.lookAt, cam.up);
	LookAtTrans = MyTransform(LookAtMat);
	if (scene->bsptree != nullptr)
	{
		//BSPObjects中包含了分割出来的新物体
		for (vector<Triangle*>::iterator i = scene->bsptree->BSPObjects.begin(); i != scene->bsptree->BSPObjects.end(); i++)
		{
			(*i)->transform = LookAtTrans* (*i)->transform;
		}
	}
	else
	{
		for (vector<Geometry*>::iterator i = scene->GeometryArray.begin(); i != scene->GeometryArray.end(); i++)
		{
			(*i)->transform = LookAtTrans* (*i)->transform;
		}

	}
	for (vector<Light*>::iterator i = scene->LightArray.begin(); i != scene->LightArray.end(); i++)
	{
		LookAtTrans.AffineTrans((*i)->pos);
	}

	//暂时的多线程优化
	for (int i = 0; i < cam.ScreenHeight-3; i+=4)
	{
		thread th1(RenderRow, scene, cam, m, i);
		thread th2(RenderRow, scene, cam, m, i+1);
		thread th3(RenderRow, scene, cam, m, i + 2);
		thread th4(RenderRow, scene, cam, m, i + 3);
		th1.join();
		th2.join();
		th3.join();
		th4.join();

		//RenderRow(scene, cam, m, i);		
		/*
		for (int j = 0; j < cam.ScreenWidth; j++)
		{

			Ray r = scene->RayThurPixel(cam, j, i, 1);
			MyColor color = scene->Raycast(r);
			FreeImage_SetPixelColor(m, j, i, &(color.GetRBGQUAD()));
		}
		*/
		printf("渲染第%d行\n", i);
		int lkj = 0;
	}
	printf("完成渲染\n");
	return m;
}





bool Scene::visibile(const Ray* ray,const Light* light) const
{
	float OLdis = vecLength(light->pos - ray->origin);
	
	vector<MyHit> SceneHits;

	if (bsptree!= nullptr)
	{
		list<Triangle*> clipTriangleList;
		GetBSPClip(ray, bsptree->root, &clipTriangleList);

		for (list<Triangle*>::const_iterator i = clipTriangleList.cbegin(); i != clipTriangleList.end(); i++)
		{
			HitPoints hps = (*i)->Intersect(ray);
			for (int x = 0; x < hps.PointCount; x++)
			{
				HitPoint hp = hps.hps[x];
				MyHit hit = MyHit(hp, *i);
				if (vecLength(hp.position - ray->origin) < OLdis)
				{
					SceneHits.push_back(hit);
				}
			}
		}

		for (vector<Sphere*>::const_iterator i = SphereArray.cbegin(); i != SphereArray.end(); i++)
		{
			HitPoints hps = (*i)->Intersect(ray);
			for (int x = 0; x < hps.PointCount; x++)
			{
				HitPoint hp = hps.hps[x];
				MyHit hit = MyHit(hp, *i);
				if (vecLength(hp.position - ray->origin) < OLdis)
				{
					SceneHits.push_back(hit);
				}
			}
		}
	}
	else
	{
		for (vector<Geometry*>::const_iterator i = GeometryArray.cbegin(); i != GeometryArray.cend(); i++)
		{
			HitPoints hps = (*i)->Intersect(ray);
			for (int x = 0; x < hps.PointCount; x++)
			{
				HitPoint hp = hps.hps[x];
				MyHit hit = MyHit(hp, *i);
				if (vecLength(hp.position - ray->origin) < OLdis)
				{
					SceneHits.push_back(hit);
				}

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



Ray Scene::RayThurPixel(const Camera* cam, int j, int i, float w) const
{
	//vec3 u = (w*tanf(radians(cam.FOVx/2) )*(-ScreenWidth + 2 * j) / ScreenWidth)*vec3(1, 0, 0);
	//vec3 v = (w*tanf(radians(cam.FOVy/2))*(-ScreenHeight + 2 * i) / ScreenHeight)*vec3(0, 1, 0);
	float sw = (float)cam->ScreenWidth;
	float sh = (float)cam->ScreenHeight;
	float aspect = sw / sh;
	float H = w*tanf(radians(cam->FOVy / 2));
	float W = H*aspect;
	vec3 u = W*(-sw + 2 * j + 1.f) / sw *vec3(1, 0, 0);  //注意是从像素中心射出，所以要加1.0作为补偿
	vec3 v = H*(-sh + 2 * i + 1.f) / sh*vec3(0, 1, 0);
	Ray r = Ray(vec3(0, 0, 0), vec3(0, 0, -1)*w + u + v, 0, MaxRayDepth);
	return r;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          