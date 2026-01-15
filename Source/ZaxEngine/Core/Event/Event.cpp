#include "Event.h"

Event& Event::operator+=(const std::function<void()> &func)
{
	list.push_back(func);
	return *this;
}

Event& Event::operator-=(const std::function<void()> &func)
{
    auto it = std::remove_if(list.begin(), list.end(),
        [&](const std::function<void()>& f) { return f.target_type() == func.target_type(); });
    list.erase(it, list.end());
	return *this;
}

void Event::Execute()
{
	for (auto e : list)
	{
		e();
	}
}
