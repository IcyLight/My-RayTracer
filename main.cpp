#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "glm/glm.hpp"
//#include "Transform.h"
#include "FreeImage.h"
#include "readfile.h"




using namespace glm;




int main()
{



	
	//readfile("scene4-ambient.test");
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-diffuse.test");
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-emission.test");
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-specular.test");
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene4-specular.test",&curScene);
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene5.test");
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	
	//readfile("scene6.test",&curScene);
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);

	//readfile("scene7.test");
	//FIBITMAP* m = Display(HWCamera);
	//FreeImage_Save(FIF_PNG, m, &curfilename[0]);



	vec3 eye = vec3(0, 30, 50);
	vec3 center = vec3(0, 30, 0);
	vec3 up = vec3(0, 1, 0);
	Camera cam = Camera(60, 60, 1, eye, center, up, 1600, 900);

	string address = string("test.obj");
	Scene* myScene = new Scene(100, 1,RenderMode::NormalMapMode);
	readfile(address.data(), myScene,LoadMode::ObjLoad);

	Light* l = new Light(normalize(cam.lookAt - cam.lookFrom), MyColor(1, 1, 1, 1), LightType::Dirctional);
	myScene->LightArray.push_back(l);

	FIBITMAP* m = Display(cam,myScene);
	FreeImage_Save(FIF_PNG, m, "Test.png");

	
	

	
	
	//FIBITMAP* m = FreeImage_Allocate(1024, 1024, 24);
	
	


	
	
	system("Pause");
}