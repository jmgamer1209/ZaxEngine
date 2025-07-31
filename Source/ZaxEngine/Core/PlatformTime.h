#pragma once

#include <chrono>

class PlatformTime
{
public:
	 static double Seconds()
	{
		 auto now = std::chrono::steady_clock::now();
		 auto ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
		 auto value = ms.time_since_epoch().count() / 1000000.0; // 获取秒数
		return value;
	}
};

