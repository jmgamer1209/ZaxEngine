#include "Component/RigidBody.h"
#include "Core/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"
#include "Physics/PhysicsSystem.h"
#include "Jolt/Physics/Collision/Shape/EmptyShape.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"

using namespace ZaxEngine::Physics;

RigidBody::RigidBody():Component()
{
	
}

void RigidBody::AddToWorld()
{
	PhysicsSystem& system = PhysicsSystem::GetInstance();
	system.OnPhysicsUpdateBegin.Add(physicsBegin);
	system.OnPhysicsUpdateEnd.Add(physicsEnd);
}

void RigidBody::RemoveFromWorld()
{
	PhysicsSystem& system = PhysicsSystem::GetInstance();
	system.OnPhysicsUpdateBegin.Remove(physicsBegin);
	system.OnPhysicsUpdateEnd.Remove(physicsEnd);
	system.GetBodyInterface().DeactivateBody(body->GetID());
	system.RemoveBody(*body);
}

void RigidBody::OnPhysicsUpdateBegin()
{
	auto collider = gameObject->GetComponent<BoxCollider>();
	if (collider == this->collider || collider == nullptr) return;
	PhysicsSystem& system = PhysicsSystem::GetInstance();

	if (body != nullptr) 
	{
		system.GetBodyInterface().DeactivateBody(body->GetID());
		system.RemoveBody(*body);
	}

	// 创建新的 body
	auto postion = gameObject->GetComponent<Transform>()->position;
	settings.mPosition = JPH::RVec3(postion.x, postion.y, postion.z);
	settings.mRotation = JPH::Quat::sIdentity();
	settings.SetShape(collider->GetShape());
	body = system.GetBodyInterface().CreateBody(settings);
	system.AddBody(*body);
	system.GetBodyInterface().ActivateBody(body->GetID());

	this->collider = collider;
}

void RigidBody::OnPhysicsUpdateEnd()
{
	if (body != nullptr) 
	{
		auto transform = this->gameObject->GetComponent<Transform>();
		auto newPos = body->GetPosition();
		transform->position = Vector3(newPos.GetX(), newPos.GetY(), newPos.GetZ());
	}
}

void RigidBody::OnAddToGO()
{
	AddToWorld();
}
