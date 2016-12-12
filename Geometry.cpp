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
	if (delta > floatEPS)
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

	/*
	*暂时有bug
	mat2 ttt = inverse(mat2(a->uvw.x, a->uvw.y, b->uvw.x, b->uvw.y));
	vec2 sv = ttt*vec2(uvw.x, uvw.y);
	float u = sv.y;
	float v = -sv.x + 1 - u;

	vec3 pos = (1-u-v)*a->pos + u*b->pos + v*c->pos;
	vec3 normal = (1 - u - v)*a->normal + u*b->normal + v*c->normal;
	return GetT2WMatrix(uvw, pos, normal);
	
	*/
	
	return MyTransform();
	
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



float Triangle::PlaneDistance2Point(const vec3* v) const //保留正负号表示方向
{
	return dot(*v - this->a->pos, this->faceNormal);
}

bool Triangle::PlaneIntersect(const Ray* ray,vec3* HitPos)  //HitPos世界（视角）坐标下的
{
	Ray r = Ray(*ray);

	r.xfrmRay(transform.trans);

	MyTransform imt = this->transform.MyInverse();
	

	vec3& d = r.dirction;
	vec3& o = r.origin;
	vec3& va = this->a->pos;

	if (glm::abs(dot(d, faceNormal)) < floatEPS)
	{
		*HitPos = vec3(0, 0, 0);
		return false;
	}

	float t = (  (dot(va, faceNormal) -dot(o,faceNormal)) / dot(d, faceNormal) ) ;

	if (t > -floatEPS && t < r.maxDepth)
	{
		r.xfrmRay(imt);
		*HitPos= o + t *d;
		return true;
	}
	else
	{
		*HitPos = vec3(0, 0, 0);
		return false;
	}
	

}

HitPoints Triangle::Intersect(const Ray* ray) const
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
	if (abs(denominator) < floatEPS) //平行的时候
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
		
		

		if(u>-floatEPS && v>-floatEPS && u + v<1+floatEPS)
		{
			vec3 hitpos = o + t*d;
			vec3 normal;
			vec3 uvw;
			if (NORMALINTERPOLATION)
			{
				normal = (1 - u - v)*a->normal + u*b->normal + v*c->normal;
				uvw = (1 - u - v)*a->uvw + u*b->uvw + v*c->uvw;
				

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


BSP_Case Triangle::GetBSPRelation(const Triangle* object, const Triangle* hyperplane)
{
	const vec3& hyperplaneNormal = hyperplane->faceNormal;
	
	float dA = hyperplane->PlaneDistance2Point(&object->a->pos);
	float dB = hyperplane->PlaneDistance2Point(&object->b->pos);
	float dC = hyperplane->PlaneDistance2Point(&object->c->pos);
	int fCount = (dA > floatEPS) + (dB > floatEPS) + (dC > floatEPS);
	int bCount = (dA < -floatEPS) + (dB < -floatEPS) + (dC < -floatEPS);
	if (fCount>=1 && bCount<=0)
	{
		return BSP_Case::front;
	}
	else if (bCount>=1 && fCount<=0)
	{
		return BSP_Case::behind;
	}
	else
	{
		return BSP_Case::span;
	}
	
}

bool Triangle::SplitTriangle(const Triangle* hyperPlane, const Triangle* Object, vector<Triangle*>* Out_Objects, vector<Vertex*>* Out_Vertexs)
{
	
	//重心坐标
	//P1 = t*V1 +(1-t)*V2 + 0*V3
	//P2 = t*V1 +(1-t)*V3 + 0*V2
	//dot(A'P1,normal)==0 && dot(A'P2,normal)==0 A'为切平面上的点
	//以上方程求得得uv
	if (GetBSPRelation(Object, hyperPlane) != BSP_Case::span)
	{
		return false;
	}
	//平面两边分别有1个点和两个点
	Vertex* soloVertex;
	Vertex* doubleVertex_A;
	Vertex* doubleVertex_B;

	const vec3& hpN = hyperPlane->faceNormal;
	const vec3& objN = Object->faceNormal;

	float dis_a = hyperPlane->PlaneDistance2Point(&Object->a->pos);
	float dis_b = hyperPlane->PlaneDistance2Point(&Object->b->pos);
	float dis_c = hyperPlane->PlaneDistance2Point(&Object->c->pos);
	

	int coinCount = (glm::abs(dis_a) < floatEPS) + (glm::abs(dis_b) < floatEPS) + (glm::abs(dis_b) < floatEPS);
	if (coinCount>=2 )
	{
		return false;
	}
	else if (coinCount == 1)
	{
		return false;
	}
	else 
	{
		
		if (dis_a*dis_b > 0)
		{
			soloVertex = Object->c;
			doubleVertex_A = Object->a;
			doubleVertex_B = Object->b;
		}
		else if (dis_a*dis_c > 0)
		{
			soloVertex = Object->b;
			doubleVertex_A = Object->a;
			doubleVertex_B = Object->c;
		}
		else if (dis_b*dis_c > 0)
		{
			soloVertex = Object->a;
			doubleVertex_A = Object->b;
			doubleVertex_B = Object->c;
		}
		else
		{
			return false;
		}
		float SdotN = dot(soloVertex->pos, hpN); //soloVertex dot Normal
		float PlaneposdotN = dot(hyperPlane->a->pos, hpN);
		float dAdotN = dot(doubleVertex_A->pos, hpN);
		float dBdotN = dot(doubleVertex_B->pos, hpN);

		float tA = (PlaneposdotN - dAdotN) / (SdotN - dAdotN) ; 
		float tB = (PlaneposdotN - dBdotN) / (SdotN - dBdotN) ; 
		Vertex* vE = new Vertex();
		Vertex* vF = new Vertex();
		vE->pos = tA*soloVertex->pos + (1 - tA)* doubleVertex_A->pos;
		vE->uvw = tA*soloVertex->uvw + (1 - tA)* doubleVertex_A->uvw;
		vE->normal = tA*soloVertex->normal + (1 - tA)* doubleVertex_A->normal;
		vF->pos = tB*soloVertex->pos + (1 - tB)* doubleVertex_B->pos;
		vF->uvw = tB*soloVertex->uvw + (1 - tB)* doubleVertex_B->uvw;
		vF->normal = tB*soloVertex->normal + (1 - tB)* doubleVertex_B->normal;
		Out_Vertexs->push_back(vE);
		Out_Vertexs->push_back(vF);
		
		Triangle* t1;
		Triangle* t2;
		Triangle* t3;

		//保证新的三角形和原来的三角形是朝向同一面的
		//this->faceNormal = normalize(cross(c->pos - a->pos, b->pos - a->pos));
		if (dot(objN, cross(vE->pos - soloVertex->pos, vF->pos - soloVertex->pos)) > floatEPS)
		{
			t1 = new Triangle(soloVertex, vF, vE,Object->m,Object->transform);
		}
		else
		{
			t1 = new Triangle(soloVertex, vE, vF, Object->m, Object->transform);
		}

		if (dot(objN, cross(doubleVertex_A->pos - vE->pos, doubleVertex_B->pos - vE->pos)) > floatEPS)
		{
			t2 = new Triangle(vE, doubleVertex_B, doubleVertex_A, Object->m, Object->transform);
		}
		else
		{
			t2 = new Triangle(vE, doubleVertex_A, doubleVertex_B, Object->m, Object->transform);
		}

		if (dot(objN, cross(vE->pos - doubleVertex_B->pos, vF->pos - doubleVertex_B->pos)) > floatEPS)
		{
			t3 = new Triangle(doubleVertex_B, vF, vE, Object->m, Object->transform);
		}
		else
		{
			t3 = new Triangle(doubleVertex_B, vE, vF, Object->m, Object->transform);
		}
		Out_Objects->push_back(t1);
		Out_Objects->push_back(t2);
		Out_Objects->push_back(t3);

		return true;
	}
}


void GetBSPClip(const Ray* ray, const TriangleBSPNode* node, list<Triangle*>* tlist)
{
	
	Ray transRay = Ray(*ray); //构造物体坐标下的射线
	transRay.xfrmRay(node->Object->transform.trans);

	vec3 RayStart = transRay.origin;
	vec3 RayEnd = transRay.origin + transRay.maxDepth* transRay.dirction;
	vec3& faceNormal = node->Object->faceNormal;
	float t1 = node->Object->PlaneDistance2Point(&RayStart);
	float t2 = node->Object->PlaneDistance2Point(&RayEnd);
	

	vec3 interPos; //视角（世界）坐标下平面与射线交会点
	bool isInter = node->Object->PlaneIntersect(ray, &interPos);
	
	//尽量保证以前中后的顺序加入链表，后面可能添加新的功能，例如快速消隐
	if (isInter)
	{
		const Ray* frontRay = ray;
		const Ray* behindRay = ray;
		Ray I2ERay = Ray(interPos, ray->dirction, ray->RecurCount, ray->maxDepth - vecLength(ray->origin - interPos)); //interPos to End
		Ray O2IRay = Ray(ray->origin, ray->dirction, ray->RecurCount, vecLength(interPos - ray->origin)); //origin to interPos
		if (t2 > floatEPS)
		{
			frontRay = &I2ERay;
			behindRay = &O2IRay;
		}
		else if(t2<-floatEPS)
		{
			frontRay = &O2IRay;
			behindRay = &I2ERay;
		}
		if (node->frontNode != nullptr)
		{
			
			GetBSPClip(frontRay, node->frontNode, tlist);
		}
		else
		{
			(*tlist).insert((*tlist).end(), node->frontObjects.begin(), node->frontObjects.end());
		}

		(*tlist).insert((*tlist).end(), node->spanObjects.begin(), node->spanObjects.end());

		if (node->behindNode != nullptr)
		{
			
			GetBSPClip(behindRay, node->behindNode, tlist);
		}
		else
		{
			(*tlist).insert((*tlist).end(), node->behindObjects.begin(), node->behindObjects.end());
		}
	}
	else
	{
		
		if (t2>floatEPS)
		{
			if (node->frontNode != nullptr)
			{
				GetBSPClip(ray, node->frontNode, tlist);
			}
			else
			{
				(*tlist).insert((*tlist).end(), node->frontObjects.begin(), node->frontObjects.end());
			}
		}
		(*tlist).insert((*tlist).end(), node->spanObjects.begin(), node->spanObjects.end());
		if(t2 < -floatEPS)
		{
			if (node->behindNode != nullptr)
			{
				
				GetBSPClip(ray, node->behindNode, tlist);
			}
			else
			{
				(*tlist).insert((*tlist).end(), node->behindObjects.begin(), node->behindObjects.end());
			}
		}
	}


	return;
}