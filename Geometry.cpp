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
	HitPoints spHit = HitPoints(); //����ֵ�����Ա�ʾ����Բ��������
								   //������Ϊ R = origin + k*dirction
	Ray r = Ray(*ray);

	r.xfrmRay(this->transform.trans);  //�����������������µ�λ�÷���

	vec3& d = r.dirction;
	vec3& o = r.origin;
	vec3& c = pos;
	float delta = 4 * dot(d, o - c)*dot(d, o - c) - 4 * dot(d, d) * (dot(o - c, o - c) - radius*radius);
	if (delta > FLOATOFFSET)
	{

		delta = sqrtf(delta);
		float k1 = (-2 * dot(d, o - c) - delta) / (2 * dot(d, d)); //k1Ϊ���㣬k2ΪԶ��
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

			//���������ǰ������

			//���k1,k2���Ҫ�����߱任���������Ұѵ��������ת������������
			MyTransform mt = this->transform;
			MyTransform imt = this->transform.MyInverse();
			r.xfrmRay(imt);

			//ת�����ӽǿռ�
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
				//��Բ��ʱ������������, �����ཻ�㿴��������Զ�ĵ�
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
	
	//��������ͼ�� tangent spaceת���ӽǿռ�
	//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/

	/*
	mat3 tt = mat3(a->uvw, b->uvw, c->uvw);
	mat3 itt = inverse(mat3(a->uvw, b->uvw, c->uvw));
	vec3 barycentric_coord = inverse(mat3(a->uvw, b->uvw, c->uvw))*uvw;
	float& u = barycentric_coord.y;
	float& v = barycentric_coord.z;
	*/

	/*
	*��ʱ��bug
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
	//��������ͼ�� tangent spaceת���ӽǿռ�
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



bool Triangle::PlaneIntersect(const Ray* ray,vec3* HitPos)  //HitPos���磨�ӽǣ������µ�
{
	Ray r = Ray(*ray);

	r.xfrmRay(transform.trans);

	MyTransform imt = this->transform.MyInverse();
	

	vec3& d = r.dirction;
	vec3& o = r.origin;
	vec3& va = this->a->pos;

	if (glm::abs(dot(d, faceNormal)) < FLOATOFFSET)
	{
		*HitPos = vec3(0, 0, 0);
		return false;
	}

	float t = (  (dot(va, faceNormal) -dot(o,faceNormal)) / dot(d, faceNormal) ) ;

	if (t > -FLOATOFFSET && t < r.maxDepth)
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
	vec3 IntFaceNormal = dot(faceNormal, r.dirction) < 0 ? faceNormal : -faceNormal;  //����ķ���ѡ��������ͬһ���
	float denominator = dot(d, IntFaceNormal);
	if (abs(denominator) < FLOATOFFSET) //ƽ�е�ʱ��
	{
		return triHit;
	}
	else
	{
		
		//�������������жϵ��Ƿ�����������

		/*
		float t = (dot(va, normal) - dot(o, normal)) / denominator;
		if (t < 0)
		{
			//HitPoint(vec3(0, 0, 0), vec3(0, 0, 0), 1, GeometryType::infinite, nullptr);

			return triHit;
		}
		vec3 planePos = o + t*d;
		*/

		//ͨ���������������֮�͵���������ķ�ʽ�жϣ�Ч�ʽϵ�,���޷�Ӧ�ò�ֵ
		/*
		float S_abc = 0.5 * sqrtf(dot( cross(va - vb,vb - vc), cross(va - vb, vb - vc)));
		float S_pab = 0.5 * sqrtf(dot( cross(planePos - va, va - vb), cross(planePos - va, va - vb)));
		float S_pac = 0.5 * sqrtf(dot( cross(planePos - va, va - vc), cross(planePos - va, va - vc)));
		float S_pbc = 0.5 * sqrtf(dot( cross(planePos - vb, vb - vc), cross(planePos - vb, vb - vc)));
		//S_abc  = S_pab+S_pac+S_pbc
		*/




		//M-T����
		//http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
		//ͨ����������,����Ҳ�Ƚϸ�,Ҳ������������������в�ֵ
		
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

		
		//������������������ж�		
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


			}
			else
			{
				normal = IntFaceNormal;
				uvw = vec3(0, 0, 0);
			}


			//���k���Ҫ�����߱任����            ���Ұѵ��������ת������������
			MyTransform mt = this->transform;
			MyTransform imt = this->transform.MyInverse();
			r.xfrmRay(imt);
			//float lenD = vecLength(d);

			//ת�����ӽǿռ�
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
	vec3 hyperplaneNormal = hyperplane->faceNormal;
	vec3 vA = object->a->pos - hyperplane->a->pos;
	vec3 vB = object->b->pos - hyperplane->a->pos;
	vec3 vC = object->c->pos - hyperplane->a->pos;
	if (dot(hyperplaneNormal, vA) > FLOATOFFSET && dot(hyperplaneNormal, vB) > FLOATOFFSET && dot(hyperplaneNormal, vC) > FLOATOFFSET)
	{
		return BSP_Case::front;
	}
	else if (dot(hyperplaneNormal, vA) < -FLOATOFFSET && dot(hyperplaneNormal, vB) < -FLOATOFFSET && dot(hyperplaneNormal, vC) < -FLOATOFFSET)
	{
		return BSP_Case::behind;
	}
	else
	{
		return BSP_Case::span;
	}
	
}





void GetBSPClip(const Ray* ray, const BSPNode<Triangle, Triangle::GetBSPRelation>* node, list<Triangle*>* tlist)
{
	Ray transRay = Ray(*ray); //�������������µ�����
	transRay.xfrmRay(node->Object->transform.trans);

	vec3 RayStart = transRay.origin;
	vec3 RayEnd = transRay.origin + transRay.maxDepth* transRay.dirction;

	vec3& faceNormal = node->Object->faceNormal;
	vec3 vS = RayStart - node->Object->a->pos;
	vec3 vE = RayEnd - node->Object->a->pos;

	vec3 interPos; //�ӽǣ����磩������ƽ�������߽����
	bool isInter = node->Object->PlaneIntersect(ray, &interPos);
	float t1 = dot(vS, faceNormal);
	float t2 = dot(vE, faceNormal);


	//DEBUG
	/*
	if (isInter && (t1*t2 > 0))
	{
		vec3 gg = vE;
		vec3 ff = faceNormal;
		DEBUG_PRINT_3VECOTOR(gg)
			DEBUG_PRINT_3VECOTOR(ff)
			printf("%f",t2);
		printf("%f", t1);
		printf("AA");
		
	}
	if (!isInter && (t1*t2 < 0))
	{
		printf("AA");
	}
	*/
	


	//������֤��ǰ�к��˳��������������������µĹ��ܣ������������
	if (isInter)
	{
		
		const Ray* frontRay = ray;
		const Ray* behindRay = ray;
		Ray I2ERay = Ray(interPos, ray->dirction, ray->RecurCount, ray->maxDepth - vecLength(ray->origin - interPos)); //interPos to End
		Ray O2IRay = Ray(ray->origin, ray->dirction, ray->RecurCount, vecLength(interPos - ray->origin)); //origin to interPos
		if (t2 > FLOATOFFSET)
		{
			frontRay = &I2ERay;
			behindRay = &O2IRay;
		}
		else if(t2<-FLOATOFFSET)
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
		if (t2>FLOATOFFSET)
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
		if(t2 < -FLOATOFFSET)
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