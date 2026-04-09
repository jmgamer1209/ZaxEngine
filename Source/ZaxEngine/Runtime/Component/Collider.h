#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "Core/GameObject.h"
#include "Core/Vector.h"
#include <vector>

class Collider: public Component
{	
public:
	virtual void SetShapeScale(Vector3 scale);
};
