#include "Event.h"

uint64_t EventListenerBase::sIndexID = 0;
std::vector<uint64_t> EventListenerBase::sReuseIDList;

EventListenerBase::EventListenerBase()
{
	if (!sReuseIDList.empty())
	{
		ID = sReuseIDList.back();
		sReuseIDList.pop_back();
	}
	else
	{
		ID = sIndexID;
		sIndexID++;
	}
}

EventListenerBase::~EventListenerBase()
{
	sReuseIDList.push_back(ID);
}