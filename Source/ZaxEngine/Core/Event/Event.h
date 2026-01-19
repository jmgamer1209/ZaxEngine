#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <mutex>
#include <algorithm>

// ===== 通用委托定义 =====
template<typename RetType, typename... Args>
using Delegate = std::function<RetType(Args...)>;

template<typename... Args>
using Action = Delegate<void, Args...>;

class EventListenerBase
{
public:
	uint64_t GetID() { return ID; }

protected:
	uint64_t ID;
	EventListenerBase();
	virtual ~EventListenerBase() = 0;

protected:
	static uint64_t sIndexID;
	static std::vector<uint64_t> sReuseIDList;
};

template <typename... Args>
class EventListener:public EventListenerBase
{
public:
	EventListener(Action<Args...> action) { 
		this->action = action;
	};
	virtual ~EventListener() { }

	void Invoke(Args... args)
	{
		this->action(args...);
	}

private:
	Action<Args...> action;
};

template <typename... Args>
class Event
{
public:
	void Add(EventListener<Args...>& listener)
	{
		list.push_back(listener);
	}

	void Remove(EventListener<Args...>& listener)
	{
		auto it = std::remove_if(list.begin(), list.end(),
			[&](EventListener<Args...>& f) { return f.GetID() == listener.GetID(); });
		list.erase(it, list.end());
	}

	void Invoke(Args... args)
	{
		for (EventListener<Args...> listener : list)
		{
			listener.Invoke(args...);
		}
	}

private:
	std::vector<EventListener<Args...>> list;
};