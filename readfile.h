#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Transform.h"
#include "Scene.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>


extern string curfilename;
enum LoadMode
{
	HwLoad, ObjLoad
};
bool readvals(stringstream &s, const int numvals, float* values);

void readfile(const char* filename, Scene* scene, LoadMode loadMode);

void HWload(const char* filename, Scene* scene);
void Objload(const char* filename, Scene* scene);
void mtlLoad(const char* filename, Scene* scene);




