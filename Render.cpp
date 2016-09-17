
#include "Render.h"

#define RECURSIVE 1
#define FLOATOFFSET 0.001

//column major

int ScreenWidth = 128;
int ScreenHeight = 128;
const float DefaultLightIntensity = 1;
double maxDepth = 100;
int RecursiveMaxDepth = 1;
Scene curScene = Scene();
Camera curCamera = Camera();
stack<MyTransform> TransformStack = stack<MyTransform>();
vec3 LightModelConstant = vec3(1, 0.1, 0.05);  //光照模型中光强随距离衰减公式的三个参数c1,c2,c3

MyTransform LookAtTrans;










//将射线由视角坐标转换到物体坐标
void Ray::xfrmRay(MyTransform ObejctTrans)
{
	ObejctTrans.trans = inverse(ObejctTrans.trans);
	mat4& AffineT = ObejctTrans.trans;
	mat3 linearT = mat3(AffineT);
	//mat3 linearT = mat3(AffineT[0][0], AffineT[0][1], AffineT[0][2],AffineT[1][0], AffineT[1][1], AffineT[1][2],AffineT[2][0], AffineT[2][1], AffineT[2][2]);
	ObejctTrans.AffineTrans(this->origin);
	this->dirction = linearT*dirction;
	
	
}

MyColor Raycast(Ray ray)
{
	if (ray.depth > RecursiveMaxDepth)
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
	MyColor color = SceneHits[0].obj->GetColor(SceneHits[0].hp.position, SceneHits[0].hp.normal, ray);

	return color;
}



bool Light::visibile(Ray ray)
{
	float OLdis = vecLength(this->pos - ray.origin);

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


	if ( SceneHits.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

HitPoints Sphere::Intersect(Ray ray)
{
	HitPoints spHit = HitPoints(); //返回值，用以表示穿过圆的两个点
	//令向量为 R = origin + k*dirction

	ray.xfrmRay(this->transform.trans);  //求射线在物体坐标下的位置方向

	vec3& d = ray.dirction;
	vec3& o = ray.origin;
	vec3& c = pos;
	float delta = 4 * dot(d, o - c)*dot(d, o - c) - 4 * dot(d, d) * (dot(o - c, o - c) - radius*radius);
	if (delta > FLOATOFFSET)
	{
		
		delta = sqrtf(delta);
		float k1 = (-2 * dot(d, o - c) - delta) / (2 * dot(d, d)); //k1为近点，k2为远点
		float k2 = (-2 * dot(d, o - c) + delta) / (2 * dot(d, d));
		bool inSphere = false;
		if (k1 < 0 && k2 < 0)
			
		{
			//spHit.hps[0]= HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite, NULL);
			return spHit;
			
		}
		else
		{
			vec3 front_hitpos = o + k1*d;
			vec3 front_normal = normalize(front_hitpos - c);
			vec3 back_hitpos = o + k2*d;
			vec3 back_normal = normalize(back_hitpos - c);
			
			//穿过球体的前后两点

			//求出k1,k2后就要吧射线变换回来，而且把点和向量都转换到世界坐标
			MyTransform mt = this->transform;
			MyTransform imt = this->transform.MyInverse();
			ray.xfrmRay(imt);

			//转换到视角空间
			mt.AffineTrans(front_hitpos);
			mt.AffineTrans(back_hitpos);
			mt.NormalTrans(front_normal);
			mt.NormalTrans(back_normal);





			double front_depth = glm::min(dvecLength(k1*d), maxDepth) / maxDepth;

			
			double back_depth = glm::min(dvecLength(k2*d), maxDepth) / maxDepth;

			if (k1*k2<0)
			{
				k1 = k1 > k2 ? k1 : k2;
				inSphere = true;
			}
			
			if (inSphere)
			{
				//在圆内时，法向量反向, 反向相交点看作是无限远的点
				//spHit.push(HitPoint(front_hitpos, -front_normal, front_depth, GeometryType::infinite, NULL));

				spHit.push(HitPoint(back_hitpos, -back_normal, back_depth, GeometryType::sph));


			}
			else
			{
				spHit.push(HitPoint(front_hitpos, front_normal, front_depth, GeometryType::sph));

				spHit.push(HitPoint(back_hitpos, back_normal, back_depth, GeometryType::sph));
			}
		}
	}
	else
	{
		//spHit.hps[0] = HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite,NULL);
		
	}


	return spHit;
}


HitPoints Triangle::Intersect(Ray ray) 
{




	HitPoints triHit = HitPoints();
	ray.xfrmRay(transform.trans);
	vec3& d = ray.dirction;
	vec3& o = ray.origin;
	vec3& va = this->a;
	vec3& vb = this->b;
	vec3& vc = this->c;
	vec3 normal = normalize(cross(vc - va, vb - va));  //两面的法线选择和入射光同一面的
	if (dot(normal, ray.dirction) > 0)
	{
		normal = -normal;
	}
	float denominator = dot(d, normal);
	if (abs(denominator) < FLOATOFFSET  )
	{
		//triHit.hps[0] = HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite, NULL);
		
		return triHit;
	}
	else
	{
		//利用重心坐标判断点是否在三角形中
		float k = (dot(va, normal) - dot(o, normal)) / denominator;
		if (k < 0)
		{
			//HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite, NULL);
			
			return triHit;
		}
		vec3 planePos = o + k*d;
		 //通过三个三角形面积之和等于总面积的方式判断，效率较低,且无法应用差值
		/*
		float S_abc = 0.5 * sqrtf(dot( cross(va - vb,vb - vc), cross(va - vb, vb - vc)));
		float S_pab = 0.5 * sqrtf(dot( cross(planePos - va, va - vb), cross(planePos - va, va - vb)));
		float S_pac = 0.5 * sqrtf(dot( cross(planePos - va, va - vc), cross(planePos - va, va - vc)));
		float S_pbc = 0.5 * sqrtf(dot( cross(planePos - vb, vb - vc), cross(planePos - vb, vb - vc)));
		//S_abc  = S_pab+S_pac+S_pbc
		*/
		

		//利用重心判断是否在三角形中 三角形中某一点P = w1 * A +w2 * B + w3 *C 只有当w1,w2,w3都大于零时才在三角形中,经过化简可得如下
		//尚有bug
		/*
		vec3 Q1 = vc - va;
		vec3 Q2 = vb - va;
		vec3 R = planePos - va;
		float Q12dot = dot(Q1, Q2);
		float sqQ1 = dot(Q1, Q1);
		float sqQ2 = dot(Q2, Q2);
		float de = sqQ1*sqQ2 - Q12dot*Q12dot;
		float RQ1dot = dot(R, Q1);
		float RQ2dot = dot(R, Q2);
		float w1 =( sqQ2 * RQ1dot - Q12dot *RQ2dot ) / de;
		float w2 = (-Q12dot *RQ1dot + sqQ1 *RQ2dot) / de;
		//w1>0 && w2>0 && w1+w1<1

		*/
		//利用向量叉积的性质判断		
		vec3 edge0 = va - vb;
		vec3 edge1 = vc - va;
		vec3 edge2 = vb - vc;
		vec3 C0 = planePos - vb;
		vec3 C1 = planePos - va;
		vec3 C2 = planePos - vc;
		float d1 = dot(normal, cross(edge0, C0));
		float d2 = dot(normal, cross(edge1, C1));
		float d3 = dot(normal, cross(edge2, C2));

		

		if( (d1>=0 &&d2>=0 && d3>=0) || (d1<=0 && d2<=0 && d3<=0) )
		{
			vec3 hitpos = o + k*d;
			//求出k后就要吧射线变换回来            而且把点和向量都转换到世界坐标
			MyTransform mt = this->transform;
			MyTransform imt = this->transform.MyInverse();
			ray.xfrmRay(imt);
			//float lenD = vecLength(d);

			//转换到视角空间
			mt.AffineTrans(hitpos);
			mt.NormalTrans(normal);


			


			double depth = glm::min(dvecLength(k*d), maxDepth) / maxDepth;
			HitPoint hp = HitPoint(hitpos, normal, depth, GeometryType::tri);
			triHit.push(hp);
			
			return triHit;
		}
		else
		{
			return triHit;
		}
	}

}


MyColor Geometry::GetColor(vec3 hitpos,vec3 normal, Ray ray)  //在视角空间中获取颜色
{
	MyColor& diffuse = this->m.diffuse;
	MyColor& specular = this->m.specular;
	MyColor& emission = this->m.emission;
	MyColor& ambient = this->m.ambient;
	float&  shininess = this->m.shininess;


	MyColor PointColor = emission+ ambient;
	



	for (vector<Light>::iterator i = curScene.LightArray.begin(); i != curScene.LightArray.end(); i++)
	{
		

		

		if (i->type == LightType::Point )
		{

			vec3 lightIn = hitpos - i->pos;
			float distance = vecLength(lightIn);
			lightIn = normalize(lightIn);
			if (i->visibile(Ray(hitpos + normalize(-lightIn)*0.001f, -lightIn, maxDepth)))
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
		PointColor = PointColor + specular*Raycast(Ray(hitpos + re*0.001f, re, ray.depth + 1));
		//注意reflect的参数I是入射方向的反向
	}

	
	return PointColor;
}






















