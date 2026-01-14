#include "Component/RigidBody.h"
#include "Core/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"
#include "Physics/PhysicsSystem.h"
using namespace ZaxEngine::Physics;

RigidBody::RigidBody():Component()
{
	
}

void RigidBody::AddToWorld()
{
	PhysicsSystem& system = PhysicsSystem::GetInstance();
	if (body == nullptr)
	{
		body = system.GetBodyInterface().CreateBody(settings);
	}
	system.AddBody(*body);
}

void RigidBody::RemoveFromWorld()
{
	PhysicsSystem& system = PhysicsSystem::GetInstance();
	system.RemoveBody(*body);
}
