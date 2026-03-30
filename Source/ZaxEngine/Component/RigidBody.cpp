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

		// 1.将引擎局部坐标转为物理局部坐标，因为变换关系非常简单，直接对 x,z 取反即可
		JPH::Vec3 physicsLocalCenter(-collider->boxCenter.x, collider->boxCenter.y, -collider->boxCenter.z);

		// 2.再将物理局部坐标转为物理世界坐标
		// 正常来说，要把一个点从模型空间转换到世界空间，本来是求出变换矩阵(或者变换四元数)，但是因为这个 center 是一个偏移量，所以没有位移和缩放的影响，只需要旋转运算即可
        JPH::Vec3 physicsWorldCenter = custom_quat * physicsLocalCenter;

		// 物理世界坐标 = 物理世界坐标 + 物理世界偏移，而物理世界坐标和引擎世界坐标，只是坐标系轴的方向不同，不存在位移和缩放，所以这里直接对 x,z 取反就是物理世界坐标
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
			auto newPhysicsPos = body->GetPosition();
			auto physicsQuat = body->GetRotation();
			auto physicsEular = Math::JPHEularAngleYXZFormJPHQuat(physicsQuat);

			JPH::Vec3 physicsLocalCenter(-collider->boxCenter.x, collider->boxCenter.y, -collider->boxCenter.z);
			JPH::Vec3 physicsWorldCenter = physicsQuat * physicsLocalCenter;

			transform->position = Vector3(
				-(newPhysicsPos.GetX() - physicsWorldCenter.GetX()),
				newPhysicsPos.GetY() - physicsWorldCenter.GetY(),
                -(newPhysicsPos.GetZ() - physicsWorldCenter.GetZ())
			);

			// physicsEular 是物理世界按照 YXZ 方向的欧拉角，因为x，z 是相反轴，所以直接取反即是引擎世界欧拉角
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