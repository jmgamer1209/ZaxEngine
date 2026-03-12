#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/GameObject.h"
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Body/Body.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Physics/PhysicsSystem.h"
#include "BoxCollider.h"

namespace ZaxEngine::Component 
{

	class RigidBody: public Component
	{	
	public:

	private:
		JPH::BodyCreationSettings settings;
		JPH::Body* body = nullptr;
		BoxCollider* collider = nullptr;
		JPH::EMotionType motionType = JPH::EMotionType::Dynamic;

	public:
		RigidBody();
		void AddToWorld();
		void RemoveFromWorld();
		void OnPhysicsUpdateBegin();
		void OnPhysicsUpdateEnd();

		EventListener<> physicsBegin = EventListener<>(std::bind(&RigidBody::OnPhysicsUpdateBegin, this));
		EventListener<> physicsEnd = EventListener<>(std::bind(&RigidBody::OnPhysicsUpdateEnd, this));
	
	public:
		void OnAddToGO() override;
		void SetMotionType(JPH::EMotionType type);
		JPH::EMotionType GetMotionType();
	};

}