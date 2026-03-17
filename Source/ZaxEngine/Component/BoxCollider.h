#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "Component.h"
#include "Collider.h"
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Core/Vector.h"

class BoxCollider: public Collider
{	
public:
	BoxCollider();

public:
	JPH::BoxShape* GetShape();
	void SetShapeScale(Vector3) override;

private:
	JPH::ShapeRefC shape;
};

