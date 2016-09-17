#include "Geometry.h"



HitPoint::HitPoint(vec3 pos, vec3 nor, double depth, GeometryType type)
{
	position = pos;
	normal = normalize(nor);
	this->depth = depth;
	this->type = type;
	//this->matieral = matieral;
}
bool HitPoint::operator< (const HitPoint& h)
{
	return this->depth < h.depth;
}
void HitPoint::Trans(MyTransform m)
{
	m.AffineTrans(this->position);
	mat3 linear = mat3(m.trans);
	this->normal = linear*this->normal;
}

bool HitPoints::push(HitPoint hp)
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


Sphere::Sphere(vec3 pos, float radius, Matieral m, MyTransform transform)
{
	this->pos = pos;
	this->radius = radius;
	this->m = m;
	this->transform = transform;

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




			
			double front_depth = glm::min(vecLength(k1*d), ray.maxDepth) / ray.maxDepth;


			double back_depth = glm::min(vecLength(k2*d), ray.maxDepth) / ray.maxDepth;

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


Triangle::Triangle(vec3 pA, vec3 pB, vec3 pC, Matieral m, MyTransform transform)
{
	this->a = pA;
	this->b = pB;
	this->c = pC;
	this->m = m;
	this->transform = transform;
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
	if (abs(denominator) < FLOATOFFSET)
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



		if ((d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0))
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





			double depth = glm::min(vecLength(k*d), ray.maxDepth) / ray.maxDepth;
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