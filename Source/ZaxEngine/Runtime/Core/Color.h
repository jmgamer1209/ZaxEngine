#pragma once

struct Color
{
	float r;
	float g;
	float b;
	float a;

public:
	Color():r(0),g(0),b(0),a(0)
	{

	}
	Color(float r, float g, float b, float a = 0) 
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float* FloatPTR()
	{
		return (float*)this;
	}
};
