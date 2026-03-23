#include "Math.h"

namespace ZaxEngine::Math
{
	/// <summary>
	/// 返回欧拉角度(注意不是弧度)
	/// 右手坐标系，X轴朝右，旋转顺序 YXZ（内旋，对应矩阵 Ry*Rx*Rz）
	/// 对应正向构造：Q = Qy * Qx * Qz，展开后：
	///   qx =  cy*sx*cz + sy*cx*sz
	///   qy =  sy*cx*cz - cy*sx*sz
	///   qz =  cy*cx*sz - sy*sx*cz
	///   qw =  cy*cx*cz + sy*sx*sz
	/// 逆推：
	///   X = asin ( 2*(w*x - y*z) )
	///   Y = atan2( 2*(w*y + z*x),  1 - 2*(x²+y²) )
	///   Z = atan2( 2*(w*z + x*y),  1 - 2*(x²+z²) )
	/// </summary>
	Vector3 EularAngleYXZFormJPHQuat(JPH::Quat& quat)
	{
        float x = quat.GetX();
        float y = quat.GetY();
        float z = quat.GetZ();
        float w = quat.GetW();

        // 1. X 轴旋转角（Pitch），YXZ 内旋中 X 是 asin 轴
        float sinX = 2.0f * (w * x - y * z);
        sinX = JPH::Clamp(sinX, -1.0f, 1.0f);
        float angleX = std::asin(sinX);

        float angleY, angleZ;

        // 2. 万向锁判断（X 接近 ±90° 时 cosX ≈ 0）
        if (JPH::abs(sinX) > 0.999999f)
        {
            // 万向锁：令 Z = 0，Y 吸收所有偏航
            angleY = std::atan2(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));
            angleZ = 0.0f;
        }
        else
        {
            // 3. 正常情况
            // Y = atan2( 2*(w*y + z*x),  1 - 2*(x²+y²) )
            angleY = std::atan2(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));
            // Z = atan2( 2*(w*z + x*y),  1 - 2*(x²+z²) )
            angleZ = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (x * x + z * z));
        }

        // 弧度转角度
        return Vector3(
            JPH::RadiansToDegrees(angleX),
            JPH::RadiansToDegrees(angleY),
            JPH::RadiansToDegrees(angleZ)
        );
	}
}