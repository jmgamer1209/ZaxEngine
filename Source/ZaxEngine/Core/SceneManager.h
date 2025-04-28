#pragma once
#include "boost/filesystem/path.hpp"
#include "Scene.h"

class SceneManager
{
public:
	static Scene* LoadScene(boost::filesystem::path path);
};

