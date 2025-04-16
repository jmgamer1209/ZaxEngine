#pragma once
#include <filesystem/path.hpp>
#include "boost/json.hpp"
using namespace boost;

class ZObject
{
	virtual void Unserialize(json::object obj);
};

