#pragma once
#include "Component/Component.h"

enum class  LightType
{
	Directional,
	Point,
	Spot
};
std::string LightTypeToString(LightType type);

class Light: public Component
{
public:
	LightType type;
	float color[3] = { 1,1,1 };
	float range;
	float innerAngle;
	float outerAngle;
	
public:
	Light(LightType type);

private:
	void OnGui() override;
};

