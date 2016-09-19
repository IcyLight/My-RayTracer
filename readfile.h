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

bool readvals(stringstream &s, const int numvals, float* values);

void readfile(const char* filename,Scene* scene);

void HWload(const char* filename, Scene* scene);
void Objload(const char* filename, Scene* scene);

enum LoadMode
{
	HwLoad,ObjLoad
};
extern LoadMode loadMode;

