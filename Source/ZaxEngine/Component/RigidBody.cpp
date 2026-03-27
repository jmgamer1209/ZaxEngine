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

		// 这里本质上是需要将渲染坐标系的旋转转换为 Jolt 坐标系的旋转，一般的做法，可以先将欧拉角转为矩阵，然后再进行轴旋转，最后转成四元数
		// 不过，这里由于轴是镜像的，所以可以直接取反，Jolt 坐标系与渲染坐标系 X、Z 轴取反，绕镜像轴旋转 θ = 绕原轴旋转 -θ
		JPH::Quat quat_y = JPH::Quat::sRotation(JPH::Vec3Arg(0,1,0),  JPH::DegreesToRadians( rotation.y));  // Y轴不变
		JPH::Quat quat_x = JPH::Quat::sRotation(JPH::Vec3Arg(1,0,0),  JPH::DegreesToRadians(-rotation.x));  // X轴镜像，角度取反
		JPH::Quat quat_z = JPH::Quat::sRotation(JPH::Vec3Arg(0,0,1),  JPH::DegreesToRadians(-rotation.z));  // Z轴镜像，角度取反

		// 内旋 YXZ：Jolt四元数左乘语义下写成 Qy*Qx*Qz
		JPH::Quat custom_quat = quat_y * quat_x * quat_z;

		// boxCenter 转到 Jolt 坐标系（X、Z 取反），再用 Jolt 四元数旋转到物理世界空间
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
			auto physicsEular = Math::EularAngleYXZFormJPHQuat(quat);

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