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
	if (body == nullptr)
	{
		auto postion = gameObject->GetComponent<Transform>()->position;
		settings.mPosition = JPH::RVec3(postion.x, postion.y, postion.z);

		JPH::EmptyShapeSettings empty_shape_settings; // (JPH::Vec3(1.0f, 1.0f, 1.0f));
		empty_shape_settings.SetEmbedded(); 
		// Create the shape
		JPH::ShapeSettings::ShapeResult empty_shape_result = empty_shape_settings.Create();
		JPH::ShapeRefC shape = empty_shape_result.Get(); 
		settings.SetShape(shape);

		body = system.GetBodyInterface().CreateBody(settings);
		system.AddBody(*body);
		system.GetBodyInterface().ActivateBody(body->GetID());

		system.OnPhysicsUpdateBegin.Add(physicsBegin);
		system.OnPhysicsUpdateEnd.Add(physicsEnd);
		//system.AddPhysicsUpdateListener((PhysicsUpdateListener*)this);
	}
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

}

void RigidBody::OnPhysicsUpdateEnd()
{
	auto transform = this->gameObject->GetComponent<Transform>(); 
	auto newPos = body->GetPosition();
	transform->position = Vector3(newPos.GetX(), newPos.GetY(), newPos.GetZ());
}

void RigidBody::OnAddToGO()
{
	AddToWorld();
}
