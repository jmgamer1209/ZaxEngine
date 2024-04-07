#pragma once
#include "Component/Component.h"
#include "Core/Color.h"
#include "Component/Transform.h"
#include "Renderer/ShadowFrameBuffer.h"
#include "Core/Utils.h"

enum class  LightType
{
	Directional = 0,
	Point = 1,
	Spot = 2
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
	int shadowMapSize = 1024;
public:
	ShadowFrameBufferBase* shadowFrameBuffer;
	
public:
	Light(LightType type);

	glm::mat4 GetViewMatrix()
	{
		return Utils::GetViewMatrix(*gameObject->GetComponent<Transform>());
	}

	glm::mat4 GetViewMatrix(int direction)
	{
		return Utils::GetViewMatrix(*gameObject->GetComponent<Transform>());
	}

	glm::mat4 GetProjectionMatrix()
	{
		if (type == LightType::Directional)
		{
			return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
		}
		else if (type == LightType::Spot)
		{
			return glm::perspective(glm::radians(outerAngle), 1.0f, 0.1f, range);
		}
		else if (type == LightType::Point)
		{
			return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, range);
		}

		return glm::mat4(1);
	}

private:
	void OnGui() override;
};

