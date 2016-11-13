#include "Geometry.h"



HitPoint::HitPoint(vec3 pos, vec3 nor, vec3 uvw, double depth, GeometryType type)
{
	position = pos;
	normal = normalize(nor);
	this->depth = depth;
	this->type = type;
	this->uvw = uvw;
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
	return true;
}


Sphere::Sphere(vec3 pos, float radius, Matieral* m, MyTransform transform)
{
	this->pos = pos;
	this->radius = radius;
	this->m = m;
	this->transform = transform;

}

HitPoints Sphere::Intersect(const Ray* ray)
{
	HitPoints spHit = HitPoints(); //返回值，用以表示穿过圆的两个点
								   //令向量为 R = origin + k*dirction
	Ray r = Ray(*ray);

	r.xfrmRay(this->transform.trans);  //求射线在物体坐标下的位置方向

	vec3& d = r.dirction;
	vec3& o = r.origin;
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
			//spHit.hps[0]= HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite, nullptr);
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
			r.xfrmRay(imt);

			//转换到视角空间
			mt.AffineTrans(front_hitpos);
			mt.AffineTrans(back_hitpos);
			mt.NormalTrans(front_normal);
			mt.NormalTrans(back_normal);




			
			double front_depth = glm::min(vecLength(k1*d), r.maxDepth) / r.maxDepth;


			double back_depth = glm::min(vecLength(k2*d), r.maxDepth) / r.maxDepth;

			if (k1*k2<0)
			{
				k1 = k1 > k2 ? k1 : k2;
				inSphere = true;
			}

			vec3 uvw = vec3(0, 0, 0);
			if (inSphere)
			{
				//在圆内时，法向量反向, 反向相交点看作是无限远的点
				//spHit.push(HitPoint(front_hitpos, -front_normal, front_depth, GeometryType::infinite, nullptr));

				spHit.push(HitPoint(back_hitpos, -back_normal,uvw, back_depth, GeometryType::sph));


			}
			else
			{
				spHit.push(HitPoint(front_hitpos, front_normal, uvw, front_depth, GeometryType::sph));

				spHit.push(HitPoint(back_hitpos, back_normal, uvw, back_depth, GeometryType::sph));
			}
		}
	}
	else
	{
		//spHit.hps[0] = HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite,nullptr);

	}


	return spHit;
}


Triangle::Triangle(Vertex* _vA, Vertex* _vB, Vertex* _vC, Matieral* m, MyTransform transform)
{
	this->a = _vA;
	this->b = _vB;
	this->c = _vC;
	this->faceNormal = normalize(cross(c->pos - a->pos, b->pos - a->pos));
	this->m = m;
	this->transform = transform;
}




MyTransform Triangle::GetT2WMatrix(const vec3& uvw) const
{
	//将法线贴图从 tangent space转到视角空间
	//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/

	/*
	mat3 tt = mat3(a->uvw, b->uvw, c->uvw);
	mat3 itt = inverse(mat3(a->uvw, b->uvw, c->uvw));
	vec3 barycentric_coord = inverse(mat3(a->uvw, b->uvw, c->uvw))*uvw;
	float& u = barycentric_coord.y;
	float& v = barycentric_coord.z;
	*/

	mat2 ttt = inverse(mat2(a->uvw.x, a->uvw.y, b->uvw.x, b->uvw.y));
	vec2 sv = ttt*vec2(uvw.x, uvw.y);
	float u = sv.y;
	float v = -sv.x + 1 - u;

	vec3 pos = (1-u-v)*a->pos + u*b->pos + v*c->pos;
	vec3 normal = (1 - u - v)*a->normal + u*b->normal + v*c->normal;
	return GetT2WMatrix(uvw, pos, normal);
	
	
	
	
	
}

MyTransform Triangle::GetT2WMatrix(const vec3& uvw, const vec3& pos, const vec3& normal) const
{
	//将法线贴图从 tangent space转到视角空间
	//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
	
	vec3 deltaPos1 = a->pos - pos;
	vec3 deltaPos2 = b->pos - pos;
	vec3 deltaUV1 = a->uvw - uvw;
	vec3 deltaUV2 = b->uvw - uvw;
	
	mat2 TBmat = inverse(mat2(deltaUV1.x, deltaUV1.y, deltaUV2.x, deltaUV2.y));

	


	vec3 T = TBmat[0][0] * deltaPos1 + TBmat[0][1] * deltaPos2;
	vec3 B = TBmat[1][0] * deltaPos1 + TBmat[1][1] * deltaPos2;

	const vec3& N = normal;
	mat3 T2WMatrix = mat3(T, B, N);
	return MyTransform((mat4)T2WMatrix);
}


HitPoints Triangle::Intersect(const Ray* ray)
{

	Ray r = Ray(*ray);


	HitPoints triHit = HitPoints();
	r.xfrmRay(transform.trans);
	vec3& d = r.dirction;
	vec3& o = r.origin;
	vec3& va = this->a->pos;
	vec3& vb = this->b->pos;
	vec3& vc = this->c->pos;
	vec3 IntFaceNormal = dot(faceNormal, r.dirction) < 0 ? faceNormal : -faceNormal;  //两面的法线选择和入射光同一面的
	float denominator = dot(d, IntFaceNormal);
	if (abs(denominator) < FLOATOFFSET) //平行的时候
	{
		return triHit;
	}
	else
	{
		//利用重心坐标判断点是否在三角形中

		/*
		float t = (dot(va, normal) - dot(o, normal)) / denominator;
		if (t < 0)
		{
			//HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite, nullptr);

			return triHit;
		}
		vec3 planePos = o + t*d;
		*/

		//通过三个三角形面积之和等于总面积的方式判断，效率较低,且无法应用差值
		/*
		float S_abc = 0.5 * sqrtf(dot( cross(va - vb,vb - vc), cross(va - vb, vb - vc)));
		float S_pab = 0.5 * sqrtf(dot( cross(planePos - va, va - vb), cross(planePos - va, va - vb)));
		float S_pac = 0.5 * sqrtf(dot( cross(planePos - va, va - vc), cross(planePos - va, va - vc)));
		float S_pbc = 0.5 * sqrtf(dot( cross(planePos - vb, vb - vc), cross(planePos - vb, vb - vc)));
		//S_abc  = S_pab+S_pac+S_pbc
		*/




		//M-T方法
		//http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
		//通过重心坐标,性能也比较高,也可以利用重心坐标进行插值
		
		vec3 E1 = vb - va;
		vec3 E2 = vc - va;
		vec3 T = o - va;
		vec3 P = cross(d, E2);
		vec3 Q = cross(T, E1);
		float PdotE1 = 1.0/dot(P, E1);
		float t = dot(Q, E2) * PdotE1;
		float u = dot(P, T) * PdotE1;
		float v = dot(Q,d) * PdotE1;
		if (t < 0)
		{
			return triHit;
		}
		
		//if(u>0 && v>0 && u+v<1 )

		
		//利用向量叉积的性质判断		
		/*
		vec3 edge0 = va - vb;
		vec3 edge1 = vc - va;
		vec3 edge2 = vb - vc;
		vec3 C0 = planePos - vb;
		vec3 C1 = planePos - va;
		vec3 C2 = planePos - vc;
		float d1 = dot(normal, cross(edge0, C0));
		float d2 = dot(normal, cross(edge1, C1));
		float d3 = dot(normal, cross(edge2, C2));
		//(d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0)
		*/
		
		

		if(u>-FLOATOFFSET && v>-FLOATOFFSET && u + v<1+FLOATOFFSET)
		{
			vec3 hitpos = o + t*d;
			vec3 normal;
			vec3 uvw;
			if (NORMALINTERPOLATION)
			{
				normal = (1 - u - v)*a->normal + u*b->normal + v*c->normal;
				uvw = (1 - u - v)*a->uvw + u*b->uvw + v*c->uvw;

				/*Debug
				printf("a %f %f %f\n", a->uvw.x, a->uvw.y, a->uvw.z);
				printf("b %f %f %f\n", b->uvw.x, b->uvw.y, b->uvw.z);
				printf("c %f %f %f\n", c->uvw.x, c->uvw.y, c->uvw.z);
				printf("uvw %f %f %f\n\n", uvw.x, uvw.y, uvw.z);
				*/
			}
			else
			{
				normal = IntFaceNormal;
				uvw = vec3(0, 0, 0);
			}


			//求出k后就要吧射线变换回来            而且把点和向量都转换到世界坐标
			MyTransform mt = this->transform;
			MyTransform imt = this->transform.MyInverse();
			r.xfrmRay(imt);
			//float lenD = vecLength(d);

			//转换到视角空间
			mt.AffineTrans(hitpos);
			mt.NormalTrans(normal);





			double depth = glm::min(vecLength(t*d), r.maxDepth) / r.maxDepth;
			HitPoint hp = HitPoint(hitpos, normal,uvw, depth, GeometryType::tri);
			triHit.push(hp);

			return triHit;
		}
		else
		{
			return triHit;
		}
	}

}