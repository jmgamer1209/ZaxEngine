#include "Light.h"
#include "imgui.h"

Light::Light(LightType type): range(10)
{
	this->type = type;
}

std::string LightTypeToString(LightType type)
{
	switch (type)
	{
	case LightType::Directional:
		return "Directional";
	case LightType::Point:
		return "Point";
	case LightType::Spot:
		return "Spot";
	default:
		return "No Type Name";
	}
}