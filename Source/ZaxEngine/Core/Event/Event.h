#pragma once
#include <vector>
#include <functional>

class Event
{
public:
	Event& operator += (const std::function<void()>&);
	Event& operator -= (const std::function<void()>&);
	void Execute();

private:
	std::vector<std::function<void()>> list;
};