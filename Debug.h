#pragma once
#include <stdio.h>

//Debug模块，定义一些Debug用的宏


#define DEBUG_PRINT_COLOR(COLOR) \
do {\
 printf("color %f %f %f\n\n", COLOR.R, COLOR.G, COLOR.B); \
}while(0); \

#define DEBUG_PRINT_STRING(string) \
do {\
 cout<<string<<endl; \
}while(0); \


#define DEBUG_PRINT_3VECOTOR(VECTOR) \
do {\
 printf("vector %f %f %f\n\n", VECTOR.x, VECTOR.y, VECTOR.z); \
}while(0); \


/*

#define DEBUG_IN_PIXEL(cx,cy,px,py,DebugMacro) \
do { \
if(cx==px && cy==py) \
{  \
	DebugMacro  \
}  \
}while(0); \

#define DEBUG_IN_RAY(cRayVal,tRayVal,DebugMacro) \
do { \
if(cRayVal == tRayVal) \
{  \
	DebugMacro  \
}  \
}while(0); \

*/