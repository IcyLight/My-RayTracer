#include "MyColor.h"



	//RGBQUAD color;
	float R, G, B, A;
	MyColor::MyColor(float R, float G, float B, float A)
	{
		this->R = R;
		this->B = B;
		this->G = G;
		this->A = A;
	}
	MyColor::MyColor() 
	{
		this->R = 0;
		this->B = 0;
		this->G = 0;
		this->A = 0;
	}
	MyColor MyColor::operator*(const float& d) const
	{
		return MyColor(R*d, G*d, B*d, A*d);
	}
	MyColor MyColor::operator+(const MyColor& mc) const
	{
		return MyColor(R + mc.R, G + mc.G, B + mc.B, A + mc.A);
	}
	MyColor MyColor::operator*(const MyColor& mc) const
	{
		return MyColor(R*mc.R, G*mc.G, B*mc.B, A*mc.A);
	}

	RGBQUAD MyColor::GetRBGQUAD()
	{
		
		float outR = glm::min(this->R, 1.f);
		float outG = glm::min(this->G, 1.f);
		float outB = glm::min(this->B, 1.f);
		float outA = glm::min(this->A, 1.f);
		RGBQUAD m;
		m.rgbBlue = (BYTE)(outB * 255);
		m.rgbGreen = (BYTE)(outG * 255);
		m.rgbRed = (BYTE)(outR * 255);
		m.rgbReserved = (BYTE)(outA * 255);
		return m;
	}
