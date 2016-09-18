#include "Ray.h"


void Ray::xfrmRay(MyTransform ObejctTrans)
{
	ObejctTrans.trans = inverse(ObejctTrans.trans);
	mat4& AffineT = ObejctTrans.trans;
	mat3 linearT = mat3(AffineT);
	ObejctTrans.AffineTrans(this->origin);
	this->dirction = linearT*dirction;


}

Ray::Ray(vec3 origin, vec3 dirction, int RecurCount, float maxDepth)
{
	this->origin = origin;
	this->dirction = normalize(dirction);
	this->RecurCount = RecurCount;
	this->maxDepth = maxDepth;
}