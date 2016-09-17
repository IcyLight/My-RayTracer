#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "glm/glm.hpp"
//#include "Transform.h"
#include "FreeImage.h"
#include "readfile.h"

//9��12�գ��ش�ʧ�󣬷���任��Ӧ��������ʱ��������������Ӧ������ʱ�Թ�����


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
	vec3 u = W*(-sw + 2 * j +1.f) / sw *vec3(1, 0, 0);  //ע���Ǵ������������������Ҫ��1.0��Ϊ����
	vec3 v = H*(-sh + 2 * i +1.f) / sh*vec3(0, 1, 0);
	Ray r = Ray(vec3(0, 0, 0), vec3(0, 0, -1)*w + u + v,0);
	return r;
}




FIBITMAP* Display(Camera cam)
{
	FIBITMAP* m = FreeImage_Allocate(ScreenWidth, ScreenHeight, 24);


	mat4 LookAtMat = lookAt(cam.lookFrom, cam.lookAt, cam.up);  //�޷��þ���ʵ��ƽ��
	LookAtTrans = MyTransform(LookAtMat);
	for (vector<Geometry*>::iterator i = curScene.GeometryArray.begin(); i != curScene.GeometryArray.end(); i++)
	{
		(*i)->transform.trans = LookAtTrans.trans* (*i)->transform.trans;  //ע������ʱģ���ӽǾ����ǲ��ھ���ջ�е�
		
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
			//printf("��Ⱦ��%d��%d�� ���߷��� %f %f %f\n",i,j, r.dirction[0], r.dirction[1], r.dirction[2]);


			//r.xfrmRay(LookAtTrans); //���ӽǿռ�ת��������ռ�
			MyColor color = Raycast(r);

			
			FreeImage_SetPixelColor(m, j, i, &(color.GetRBGQUAD()) );
		}
		printf("��Ⱦ��%d��\n", i);
		int lkj = 0;
	}
	printf("�����Ⱦ\n");
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