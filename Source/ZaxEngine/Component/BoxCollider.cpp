#include "Component/BoxCollider.h"
#include "Core/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"

BoxCollider::BoxCollider():boxSize(1.0f)
{
	
}

void BoxCollider::CreateShape()
{
	JPH::Vec3 half = JPH::Vec3(boxSize.x/2.0f, boxSize.y/2.0f, boxSize.z/2.0f);
	JPH::BoxShapeSettings shape_settings(half);
	// shape_settings.SetEmbedded();
	// Create the shape
	JPH::ShapeSettings::ShapeResult empty_shape_result = shape_settings.Create();
	JPH::ShapeRefC shape = empty_shape_result.Get();
	//shape.GetPtr().
	this->shape = shape;
}

JPH::BoxShape* BoxCollider::GetShape()
{
	return (JPH::BoxShape*)shape.GetPtr();
}

void BoxCollider::SetShapeScale(Vector3 scale)
{
	shape.GetPtr()->ScaleShape(JPH::Vec3(scale.x, scale.y, scale.z));
}

void BoxCollider::SetSize(Vector3 size)
{
	this->boxSize = size;
}

BoxCollider::~BoxCollider()
{
	auto x = this->boxSize.x;
}
