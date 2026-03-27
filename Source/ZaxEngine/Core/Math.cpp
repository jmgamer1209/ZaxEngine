#include "Math.h"

namespace ZaxEngine::Math
{
	/// <summary>
	/// 返回物理世界的欧拉角度(注意不是弧度)
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
	Vector3 JPHEularAngleYXZFormJPHQuat(JPH::Quat& quat)
	{
        float x = quat.GetX();
        float y = quat.GetY();
        float z = quat.GetZ();
        float w = quat.GetW();

        float sinX = JPH::Clamp(2.0f * (w * x - y * z), -1.0f, 1.0f);
        float angleX = std::asin(sinX);
        float angleY = std::atan2(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));
        float angleZ = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (x * x + z * z));

        return Vector3(
            JPH::RadiansToDegrees(angleX),
            JPH::RadiansToDegrees(angleY),
            JPH::RadiansToDegrees(angleZ)
        );
        
         ///////////////////////////////////////////////////////////////////
         // 如果带万向锁分支，那么下面是平滑插值过程，可以解决x轴逼近90度时的跳变问题
         //////////////////////////////////////////////////////////////////
        //float x = quat.GetX();
        //float y = quat.GetY();
        //float z = quat.GetZ();
        //float w = quat.GetW();

        //// 1. X 轴旋转角（Pitch），YXZ 内旋中 X 是 asin 轴
        //float sinX = 2.0f * (w * x - y * z);
        //sinX = JPH::Clamp(sinX, -1.0f, 1.0f);
        //float angleX = std::asin(sinX);

        //// 2. 正常分支公式
        //float angleY_normal = std::atan2(2.0f * (w * y + z * x), 1.0f - 2.0f * (x * x + y * y));
        //float angleZ_normal = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (x * x + z * z));

        //// 3. 万向锁分支公式（X = ±90° 时，Z 自由度丢失，令 Z = 0）
        //float sign = (sinX >= 0.0f) ? 1.0f : -1.0f;
        //float angleY_gimbal = std::atan2(sign * 2.0f * (w * y - x * z), 1.0f - 2.0f * (y * y + z * z));
        //float angleZ_gimbal = 0.0f;

        //// 4. 用 smoothstep 在过渡区插值，避免硬切换跳变
        //// absX 越接近 1（X 越接近 ±90°），越偏向万向锁公式
        //float absX = JPH::abs(sinX);
        //// 过渡区间：[0.98, 1.0]，在此区间内平滑混合
        //float t = JPH::Clamp((absX - 0.98f) / (1.0f - 0.98f), 0.0f, 1.0f);
        //t = t * t * (3.0f - 2.0f * t); // smoothstep

        //// 插值前需要处理 atan2 的角度连续性（两个 Y 值可能差 2π）
        //auto wrapAngle = [](float target, float ref) -> float {
        //    float diff = target - ref;
        //    while (diff >  JPH::JPH_PI) diff -= 2.0f * JPH::JPH_PI;
        //    while (diff < -JPH::JPH_PI) diff += 2.0f * JPH::JPH_PI;
        //    return ref + diff;
        //};
        //angleY_gimbal = wrapAngle(angleY_gimbal, angleY_normal);
        //angleZ_gimbal = wrapAngle(angleZ_gimbal, angleZ_normal);

        //float angleY = angleY_normal + t * (angleY_gimbal - angleY_normal);
        //float angleZ = angleZ_normal + t * (angleZ_gimbal - angleZ_normal);

        //// 弧度转角度
        //return Vector3(
        //    JPH::RadiansToDegrees(angleX),
        //    JPH::RadiansToDegrees(angleY),
        //    JPH::RadiansToDegrees(angleZ)
        //);
	}

    /// <summary>
    /// 从引擎的欧拉角(注意不是弧度)转换为Jolt物理世界的四元数
    /// </summary>
    /// <param name="eular"></param>
    /// <returns></returns>
    JPH::Quat JPHQuatFromEularAngleYXZ(Vector3& eular)
	{
        // 对于空间变换，一般情况，可以先计算出自身坐标系的四元数，然后根据 最终四元数 = q_basis * 你的旋转四元数 * q_basis的逆 来计算出最终四元数，当然利用矩阵也可以
        // 不过，这里由于轴是镜像的，所以可以直接取反，Jolt 坐标系与渲染坐标系 X、Z 轴取反，绕镜像轴旋转 θ = 绕原轴旋转 -θ
        JPH::Quat quat_y = JPH::Quat::sRotation(JPH::Vec3Arg(0, 1, 0), JPH::DegreesToRadians(eular.y));  // Y轴不变
        JPH::Quat quat_x = JPH::Quat::sRotation(JPH::Vec3Arg(1, 0, 0), JPH::DegreesToRadians(-eular.x));  // X轴镜像，角度取反
        JPH::Quat quat_z = JPH::Quat::sRotation(JPH::Vec3Arg(0, 0, 1), JPH::DegreesToRadians(-eular.z));  // Z轴镜像，角度取反

        // 内旋 YXZ：Jolt四元数左乘语义下写成 Qy*Qx*Qz
        JPH::Quat custom_quat = quat_y * quat_x * quat_z;
        return custom_quat;

        // 下面是直接计算四元数的公式，效率更高，不过暂时先不用
		/*float x = JPH::DegreesToRadians(eular.x);
		float y = JPH::DegreesToRadians(eular.y);
		float z = JPH::DegreesToRadians(eular.z);

		float cy = std::cos(y * 0.5f);
		float sy = std::sin(y * 0.5f);
		float cp = std::cos(x * 0.5f);
		float sp = std::sin(x * 0.5f);
		float cr = std::cos(z * 0.5f);
		float sr = std::sin(z * 0.5f);

		float qw = cy * cp * cr + sy * sp * sr;
		float qx = cy * sp * cr + sy * cp * sr;
		float qy = sy * cp * cr - cy * sp * sr;
		float qz = cy * cp * sr - sy * sp * cr;

		return JPH::Quat(qx, qy, qz, qw);*/
	}
}