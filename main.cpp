#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "glm/glm.hpp"
//#include "Transform.h"
#include "FreeImage.h"
#include "readfile.h"



using namespace glm;













FIBITMAP* Display(Camera cam)
{
	FIBITMAP* m = FreeImage_Allocate(cam.ScreenWidth, cam.ScreenHeight, 24);


	mat4 LookAtMat = lookAt(cam.lookFrom, cam.lookAt, cam.up); 
	LookAtTrans = MyTransform(LookAtMat);
	for (vector<Geometry*>::iterator i = curScene.GeometryArray.begin(); i != curScene.GeometryArray.end(); i++)
	{
		(*i)->transform.trans = LookAtTrans.trans* (*i)->transform.trans; 
		
	}
	for (vector<Light>::iterator i = curScene.LightArray.begin(); i != curScene.LightArray.end(); i++)
	{
		LookAtTrans.AffineTrans(i->pos);
	}
	

	for (int i = 0; i < cam.ScreenHeight; i++)
	{
		for (int j = 0; j < cam.ScreenWidth; j++)
		{
			
			Ray r = curScene.RayThurPixel(cam, j, i, 1);


			MyColor color = curScene.Raycast(r);

			
			FreeImage_SetPixelColor(m, j, i, &(color.GetRBGQUAD()) );
		}
		printf("渲染第%d行\n", i);
		int lkj = 0;
	}
	printf("完成渲染\n");
	return m;
}





int main()
{
	mat4 tmd = mat4(1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4);
	mat3 j = mat3(tmd);
	curScene = Scene(100, 1);;
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


	
	

	//Camera cam = Camera(60, 60, 1,eye,center,up);
	
	//FIBITMAP* m = FreeImage_Allocate(1024, 1024, 24);
	
	


	
	
	system("Pause");
}