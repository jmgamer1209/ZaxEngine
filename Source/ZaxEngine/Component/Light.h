#pragma once
#include "Component/Component.h"
#include "Core/Color.h"
#include "Component/Transform.h"
#include "Renderer/ShadowFrameBuffer.h"

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
	int shadowMapSize = 2048;
public:
	ShadowFrameBuffer* shadowFrameBuffer;
	
public:
	Light(LightType type);
	glm::mat4 GetViewMatrix()
	{
		glm::mat4 viewRotation(1.0f);
		auto transform = gameObject->GetComponent<Transform>();
		auto position = transform->position;
		auto rotation = transform->rotation;

		viewRotation = glm::rotate(viewRotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		viewRotation = glm::rotate(viewRotation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		viewRotation = glm::rotate(viewRotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		viewRotation = glm::transpose(viewRotation);

		glm::mat4 viewTranslation(1.0f);
		viewTranslation = glm::translate(viewTranslation, glm::vec3(-position.x, -position.y, -position.z));

		return viewRotation * viewTranslation;
	}

	glm::mat4 GetProjectionMatrix()
	{
		if (type == LightType::Directional)
		{
			return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 50.0f);
		}
		if (type == LightType::Spot)
		{
			return glm::perspective(glm::radians(outerAngle), 1.0f, 0.1f, range);
		}
		return glm::mat4(1);
	}

private:
	void OnGui() override;
};

