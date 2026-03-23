#include "Component/RigidBody.h"
#include "Core/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"
#include "Physics/PhysicsSystem.h"
#include "Jolt/Physics/Collision/Shape/EmptyShape.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Core/Math.h"

using namespace ZaxEngine::Physics;

namespace ZaxEngine::Component {

	RigidBody::RigidBody() :Component()
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

		collider->CreateShape();
		// 创建新的 body
		auto t = gameObject->GetComponent<Transform>();
		auto postion = t->position;
		auto rotation = t->rotation;

		JPH::Quat quat_y = JPH::Quat::sRotation(JPH::Vec3Arg(0,1,0), JPH::DegreesToRadians(rotation.y));  // 绕Y轴
		JPH::Quat quat_x = JPH::Quat::sRotation(JPH::Vec3Arg(1,0,0), JPH::DegreesToRadians(rotation.x));  // 绕X轴
		JPH::Quat quat_z = JPH::Quat::sRotation(JPH::Vec3Arg(0,0,1), JPH::DegreesToRadians(rotation.z));  // 绕Z轴
		// 内旋 YXZ：对应矩阵 Ry*Rx*Rz，Jolt四元数左乘语义下需写成 Qy*Qx*Qz
		JPH::Quat custom_quat = quat_y * quat_x * quat_z;
		
		settings.mPosition = JPH::RVec3(postion.x, postion.y, postion.z);
		settings.mRotation = custom_quat;
		settings.SetShape(collider->GetShape());
		settings.mObjectLayer = Layers::MOVING;
		body = system.GetBodyInterface().CreateBody(settings);
		body->SetMotionType(motionType);
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
			auto quat = body->GetRotation();
			auto rotation = Math::EularAngleYXZFormJPHQuat(quat);
			transform->position = Vector3(newPos.GetX(), newPos.GetY(), newPos.GetZ());
			transform->rotation = rotation;
		}
	}

	void RigidBody::OnAddToGO()
	{
		AddToWorld();
	}

	void RigidBody::SetMotionType(JPH::EMotionType type)
	{
		this->motionType = type;
		if (body) 
		{
			this->body->SetMotionType(motionType);
		}
	}

	JPH::EMotionType RigidBody::GetMotionType()
	{
		return motionType;
	}
}