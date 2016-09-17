#include "Ray.h"


void Ray::xfrmRay(MyTransform ObejctTrans)
{
	ObejctTrans.trans = inverse(ObejctTrans.trans);
	mat4& AffineT = ObejctTrans.trans;
	mat3 linearT = mat3(AffineT);
	//mat3 linearT = mat3(AffineT[0][0], AffineT[0][1], AffineT[0][2],AffineT[1][0], AffineT[1][1], AffineT[1][2],AffineT[2][0], AffineT[2][1], AffineT[2][2]);
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