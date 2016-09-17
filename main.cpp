#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "glm/glm.hpp"
//#include "Transform.h"
#include "FreeImage.h"
#include "readfile.h"

//9月12日：重大失误，仿射变换不应该再输入时对物体做，而是应该在求交时对光线做


using namespace glm;








Ray RayThurPixel(Camera cam,int j,int i,float w)
{
	//vec3 u = (w*tanf(radians(cam.FOVx/2) )*(-ScreenWidth + 2 * j) / ScreenWidth)*vec3(1, 0, 0);
	//vec3 v = (w*tanf(radians(cam.FOVy/2))*(-ScreenHeight + 2 * i) / ScreenHeight)*vec3(0, 1, 0);
	float sw = (float)ScreenWidth;
	float sh = (float)ScreenHeight;
	float aspect = sw / sh;
	float H = w*tanf(radians(cam.FOVy / 2));
	float W = H*aspect;
	vec3 u = W*(-sw + 2 * j +1.f) / sw *vec3(1, 0, 0);  //注意是从像素中心射出，所以要加1.0作为补偿
	vec3 v = H*(-sh + 2 * i +1.f) / sh*vec3(0, 1, 0);
	Ray r = Ray(vec3(0, 0, 0), vec3(0, 0, -1)*w + u + v,0);
	return r;
}




FIBITMAP* Display(Camera cam)
{
	FIBITMAP* m = FreeImage_Allocate(ScreenWidth, ScreenHeight, 24);


	mat4 LookAtMat = lookAt(cam.lookFrom, cam.lookAt, cam.up);  //无法用矩阵实现平移
	LookAtTrans = MyTransform(LookAtMat);
	for (vector<Geometry*>::iterator i = curScene.GeometryArray.begin(); i != curScene.GeometryArray.end(); i++)
	{
		(*i)->transform.trans = LookAtTrans.trans* (*i)->transform.trans;  //注意输入时模型视角矩阵是不在矩阵栈中的
		
	}
	/*
	for (vector<Triangle>::iterator i = curScene.triArray.begin(); i != curScene.triArray.end(); i++)
	{
		i->transform.trans = LookAtTrans.trans*i->transform.trans;
	}
	*/
	
	for (vector<Light>::iterator i = curScene.LightArray.begin(); i != curScene.LightArray.end(); i++)
	{
		LookAtTrans.AffineTrans(i->pos);
	}
	

	for (int i = 0; i < ScreenHeight; i++)
	{
		for (int j = 0; j < ScreenWidth; j++)
		{
			
			Ray r = RayThurPixel(cam, j, i, 1);
			//printf("渲染第%d行%d列 射线方向 %f %f %f\n",i,j, r.dirction[0], r.dirction[1], r.dirction[2]);


			//r.xfrmRay(LookAtTrans); //由视角空间转换到世界空间
			MyColor color = Raycast(r);

			
			FreeImage_SetPixelColor(m, j, i, &(color.GetRBGQUAD()) );
		}
		printf("渲染第%d行\n", i);
		int lkj = 0;
	}
	printf("完成渲染\n");
	return m;
}



//Matieral m1 = Matieral( MyColor{ 10,10,10,0 }, MyColor{ 100,100,100,0 }, MyColor{ 40,40,40,0 }, 20);
//Matieral m2 = Matieral( MyColor{ 0,0,10,0 }, MyColor{ 0,0,160,0 }, MyColor{ 0,0,40,0 }, 20);
//Sphere s1 = Sphere(vec3(0, 0, -3), 0.4, &m2);
//Sphere s2 = Sphere(vec3(-1, 1, -2), 0.2, &m1);
//Triangle t1 = Triangle(vec3(0.5, 0.5, -4), vec3(8, 0, 0), vec3(0, 8, 0),&m2);
//Triangle t2 = Triangle(vec3(0, 0, -1), vec3(-0.2, -0.2, -1), vec3(0, -0.2, -1), &m1);
//Light l = Light(vec3(0,0, 0), MyColor{ 100,100,100,0 }, 8,LightType::Point);


int main()
{
	mat4 tmd = mat4(1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4);
	mat3 j = mat3(tmd);
	curScene = Scene();
	vec3 eye = vec3(0.5, 0, 0);
	vec3 center = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);

	//readfile("scene4-ambient.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-diffuse.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-emission.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-specular.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-specular.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene5.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	
	readfile("scene6.test");
	FIBITMAP* m = Display(curCamera);
	FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene7.test");
	//FIBITMAP* m = Display(curCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//curScene.triArray.push_back(t1);
	//curScene.triArray.push_back(t2);
	//curScene.sphArray.push_back(s1);
	//curScene.sphArray.push_back(s2);
	//curScene.LightArray.push_back(l);
	
	

	//Camera cam = Camera(60, 60, 1,eye,center,up);
	
	//FIBITMAP* m = FreeImage_Allocate(1024, 1024, 24);
	
	


	
	
	system("Pause");
}