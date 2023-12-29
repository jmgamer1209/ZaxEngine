#pragma once
#include "Component/Component.h"

enum class  LightType
{
	Directional,
	Point
};
std::string LightTypeToString(LightType type);

class Light: public Component
{
public:
	LightType type;
	float color[3] = { 1,1,1 };
	float range;
	
public:
	Light(LightType type);

private:
	void OnGui() override;
};

