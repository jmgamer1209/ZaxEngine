#include "Component/BoxCollider.h"
#include "Core/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"

BoxCollider::BoxCollider()
{
	JPH::BoxShapeSettings shape_settings(JPH::Vec3(1.0f, 1.0f, 1.0f));
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
