#pragma once
#include "Jolt/Jolt.h"
#include "Jolt/Core/Core.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/Body/BodyActivationListener.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include "Core/Event/Event.h"
#include <iostream>
#include <cstdarg>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;
using uint = JPH::uint;
using uint64 = JPH::uint64;
using Body = JPH::Body;
using RVec3Arg = JPH::RVec3Arg;
class PhysicsUpdateListener;
class Collider;

namespace ZaxEngine::Physics
{

	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;      // 此layer层之间的碰撞，默认不触发碰撞事件
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};

	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr uint NUM_LAYERS(2);
	};

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};

	// BroadPhaseLayerInterface implementation
	// This defines a mapping between object and broadphase layers.
	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl();


		virtual uint					GetNumBroadPhaseLayers() const override;

		virtual JPH::BroadPhaseLayer			GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	/// Class that determines if two object layers can collide
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};

	class MyBodyActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void		OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;

		virtual void		OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData) override;
	};

	class MyContactListener : public JPH::ContactListener
	{
	public:
		// See: ContactListener
		virtual JPH::ValidateResult	OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;

		virtual void			OnContactAdded(const Body& inBody1, const Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;


		virtual void			OnContactPersisted(const Body& inBody1, const Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

		virtual void			OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;
	};

	class PhysicsUpdateListener
	{
		virtual void OnPhysicsUpdateBegin() = 0;
		virtual void OnPhysicsUpdateEnd() = 0;
	};

	// 碰撞事件对，用于延迟到主线程处理
	struct CollisionEvent
	{
		Collider* self;
		Collider* other;
	};

	class PhysicsSystem
	{
		friend MyContactListener;

	public:
		static PhysicsSystem& GetInstance();
		void AddBody(Body& body);
		void RemoveBody(Body& body);
		JPH::BodyInterface& GetBodyInterface();
		void Update(float inDeltaTime);
		
		Event<> OnPhysicsUpdateBegin;
		Event<> OnPhysicsUpdateEnd;
		/*void AddPhysicsUpdateListener(PhysicsUpdateListener* listener);
		void RemovePhysicsUpdateListener(PhysicsUpdateListener* listener);*/

	private:
		static PhysicsSystem* instance;

		JPH::TempAllocatorImpl* temp_allocator = nullptr;
		JPH::PhysicsSystem joltPhysicsSystem;
		JPH::JobSystemThreadPool joltJobSystem;
		BPLayerInterfaceImpl broad_phase_layer_interface;
		ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
		ObjectLayerPairFilterImpl object_vs_object_layer_filter;
		MyBodyActivationListener body_activation_listener;
		MyContactListener contact_listener;
		//std::vector<PhysicsUpdateListener*> physicsUpdateListenerList;

	private:
		PhysicsSystem();

		// 待处理的碰撞事件队列（线程安全）
		std::vector<CollisionEvent> pendingCollisionEvents;
		std::mutex collisionEventMutex;

		// 由物理线程调用，缓存碰撞事件
		void EnqueueCollisionEvent(Collider* self, Collider* other);

		// 在主线程处理所有缓存的碰撞事件
		void ProcessPendingCollisionEvents();

		// 实际执行碰撞回调（仅在主线程调用）
		void DispatchColliderEnter(Collider* self, Collider* other);

	private:
		static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine);
		static void TraceImpl(const char* inFMT, ...);
	};

}


