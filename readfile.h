#pragma once

#include "Transform.h"
#include "Scene.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

extern string curfilename;

bool readvals(stringstream &s, const int numvals, float* values);

void readfile(const char* filename);

