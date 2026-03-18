#include "PhysicsSystem.h"
#include "Jolt/Physics/EActivation.h"
#include "Jolt/Core/Memory.h"
#include "Jolt/Core/IssueReporting.h"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/TempAllocator.h"
#include <Core/Debug.h>

namespace ZaxEngine::Physics
{
	PhysicsSystem* PhysicsSystem::instance = nullptr;

	PhysicsSystem::PhysicsSystem()
	{
		JPH::RegisterDefaultAllocator();
		JPH::Trace = TraceImpl;
		JPH::JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)
			JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();
		temp_allocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
		joltJobSystem.Init(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

		// 初始化并设置 Jolt Physics System
		{
			const uint cMaxBodies = 1024;
			const uint cNumBodyMutexes = 0;
			const uint cMaxBodyPairs = 1024;
			const uint cMaxContactConstraints = 1024;
			joltPhysicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);
			joltPhysicsSystem.SetBodyActivationListener(&body_activation_listener);
			joltPhysicsSystem.SetContactListener(&contact_listener);
		}
	}

	PhysicsSystem& PhysicsSystem::GetInstance()
	{
		// 未加锁
		if (instance == nullptr)
		{
			instance = new PhysicsSystem();
		}
		return *instance;
	}

	void PhysicsSystem::Update(float inDeltaTime)
	{
		OnPhysicsUpdateBegin.Invoke();

		joltPhysicsSystem.Update(inDeltaTime, 1, temp_allocator, &joltJobSystem);
		//joltPhysicsSystem.sim

		OnPhysicsUpdateEnd.Invoke();
	}

	void PhysicsSystem::AddBody(Body& body)
	{
		JPH::BodyInterface& body_interface = joltPhysicsSystem.GetBodyInterface();
		body_interface.AddBody(body.GetID(), JPH::EActivation::DontActivate);
	}

	void PhysicsSystem::RemoveBody(Body& body)
	{
		JPH::BodyInterface& body_interface = joltPhysicsSystem.GetBodyInterface();
		body_interface.RemoveBody(body.GetID());
	}

	JPH::BodyInterface& PhysicsSystem::GetBodyInterface()
	{
		return joltPhysicsSystem.GetBodyInterface();
	}

	//void PhysicsSystem::AddPhysicsUpdateListener(PhysicsUpdateListener* listener)
	//{
	//	physicsUpdateListenerList.push_back(listener);
	//}

	//void PhysicsSystem::RemovePhysicsUpdateListener(PhysicsUpdateListener* listener)
	//{

	//}


	bool PhysicsSystem::AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// Print to the TTY
		cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << endl;

		// Breakpoint
		return true;
	};

	void PhysicsSystem::TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		// Print to the TTY
		cout << buffer << endl;
	}


	bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}

	BPLayerInterfaceImpl::BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	uint BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* BPLayerInterfaceImpl::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED


	bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}


	void MyBodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData)
	{
		cout << "A body got activated" << endl;
	}

	void MyBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData)
	{
		cout << "A body went to sleep" << endl;
	}

	JPH::ValidateResult MyContactListener::OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
	{
		cout << "Contact validate callback" << endl;

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	void	MyContactListener::OnContactAdded(const Body& inBody1, const Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		Debug::Log("A contact was added");
	}

	void			MyContactListener::OnContactPersisted(const Body& inBody1, const Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		cout << "A contact was persisted" << endl;
	}

	void			MyContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
	{
		cout << "A contact was removed" << endl;
	}
};