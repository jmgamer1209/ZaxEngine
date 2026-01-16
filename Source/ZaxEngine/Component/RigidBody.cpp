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
		auto postion = gameObject->GetComponent<Transform>()->position;
		settings.mPosition = JPH::RVec3(postion.x, postion.y, postion.z);
		body = system.GetBodyInterface().CreateBody(settings);
		system.OnPhysicsUpdateBegin.Add(physicsBegin);
		system.OnPhysicsUpdateEnd.Add(physicsEnd);
		//system.AddPhysicsUpdateListener((PhysicsUpdateListener*)this);
	}
	system.AddBody(*body);
}

void RigidBody::RemoveFromWorld()
{
	PhysicsSystem& system = PhysicsSystem::GetInstance();
	system.OnPhysicsUpdateBegin.Remove(physicsBegin);
	system.OnPhysicsUpdateEnd.Remove(physicsEnd);
	system.RemoveBody(*body);
}

void RigidBody::OnPhysicsUpdateBegin()
{

}

void RigidBody::OnPhysicsUpdateEnd()
{

}