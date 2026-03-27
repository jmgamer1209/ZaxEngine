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
	~BoxCollider();
	void CreateShape();

public:
	JPH::BoxShape* GetShape();
	void SetShapeScale(Vector3) override;
	void SetSize(Vector3);
	void SetBoxCenter(Vector3);

public:
	Vector3 boxCenter;

private:
	JPH::ShapeRefC shape;
	Vector3 boxSize;
    
};

