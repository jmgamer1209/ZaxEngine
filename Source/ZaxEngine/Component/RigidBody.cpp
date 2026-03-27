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

		// 获取物理世界变换四元数
		auto custom_quat = Math::JPHQuatFromEularAngleYXZ(rotation);

		// boxCenter 转到 Jolt 坐标系（X、Z 取反），再用 Jolt 四元数旋转到物理世界空间
		// 正常来说，要把一个点从一个空间转换到另一个空间，本来是求出变换矩阵(或者变换四元数)，对原位置变换，但是因为位置关系比较简单，可以直接取反即可，上面又求出了旋转四元数，这样组合就等同于变换矩阵
		JPH::Vec3 localCenter(-collider->boxCenter.x, collider->boxCenter.y, -collider->boxCenter.z);
        JPH::Vec3 physicsWorldCenter = custom_quat * localCenter;

		settings.mPosition = JPH::RVec3(
			-postion.x + physicsWorldCenter.GetX(),
			 postion.y + physicsWorldCenter.GetY(),
			-postion.z + physicsWorldCenter.GetZ()
		);
		settings.mRotation = custom_quat;
		settings.mFriction = 0.8f;
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
			auto physicsEular = Math::JPHEularAngleYXZFormJPHQuat(quat);

			// boxCenter 转到 Jolt 坐标系，用 Jolt 四元数旋转后还原渲染坐标系位置
			JPH::Vec3 localCenter(-collider->boxCenter.x, collider->boxCenter.y, -collider->boxCenter.z);
			JPH::Vec3 physicsWorldCenter = quat * localCenter;

			transform->position = Vector3(
				-(newPos.GetX() - physicsWorldCenter.GetX()),
                 newPos.GetY() - physicsWorldCenter.GetY(),
                -(newPos.GetZ() - physicsWorldCenter.GetZ())
			);
			transform->rotation = Vector3(
				-physicsEular.x,
				physicsEular.y,
				-physicsEular.z
			);
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