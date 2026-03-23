#pragma once

#include "ThirdParty/glm/glm.hpp"
#include <math.h>
#include "JoltPhysics/Jolt/Jolt.h"
#include "JoltPhysics/Jolt/Math/Quat.h"
#include "Vector.h"

namespace ZaxEngine::Math
{
	/// <summary>
	/// 获取欧拉顺序为YXZ的角度
	/// </summary>
	/// <param name="quat"></param>
	/// <returns></returns>
	Vector3 EularAngleYXZFormJPHQuat( JPH::Quat& quat);
}