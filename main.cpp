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




	vec3 eye = vec3(0, 40, 80);
	vec3 center = vec3(0, 40, 0);
	vec3 up = vec3(0, 1, 0);
	Camera cam = Camera(60, 60, 1, eye, center, up, 1920, 1080);

	string address = string("test.obj");
	Scene* myScene = new Scene(100, 1,RenderMode::NormalMapMode);
	readfile(address.data(), myScene,LoadMode::ObjLoad);

	Light* l = new Light(normalize(cam.lookAt - cam.lookFrom), MyColor(1, 1, 1, 1), LightType::Dirctional);
	myScene->LightArray.push_back(l);




	FIBITMAP* m = Display(cam,myScene);
	FreeImage_Save(FIF_PNG, m, "Test.png");

	system("Pause");
}