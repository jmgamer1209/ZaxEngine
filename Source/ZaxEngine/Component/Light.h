#pragma once
#include "Component/Component.h"
#include "Core/Color.h"

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
	Color color = { 1,1,1 };
	float range;
	float innerAngle;
	float outerAngle;
	float shadowDepthBias = 1;
	
public:
	Light(LightType type);

private:
	void OnGui() override;
};

