#pragma once
#include <filesystem/path.hpp>
#include "boost/json.hpp"
using namespace boost;
#include <unordered_map>

class ZObject
{
public:
	virtual void Unserialize(json::object obj);
};

