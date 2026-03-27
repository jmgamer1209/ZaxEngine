#pragma once

#include "ThirdParty/glm/glm.hpp"
#include <math.h>
#include "JoltPhysics/Jolt/Jolt.h"
#include "JoltPhysics/Jolt/Math/Quat.h"
#include "Vector.h"

namespace ZaxEngine::Math
{
	/// <summary>
	/// 从物理世界的四元数，获取物理世界欧拉顺序为YXZ的角度
	/// </summary>
	/// <param name="quat"></param>
	/// <returns></returns>
	Vector3 JPHEularAngleYXZFormJPHQuat( JPH::Quat& quat);


	/// <summary>
	/// 从引擎的欧拉角(注意不是弧度)转换为Jolt物理世界的四元数
	/// </summary>
	/// <param name="eular"></param>
	/// <returns></returns>
	JPH::Quat JPHQuatFromEularAngleYXZ(Vector3& eular);
}