#include "SceneManager.h"
#include "Utils.h"

Scene*  SceneManager::mainScene = nullptr;

Scene* SceneManager::LoadScene(boost::filesystem::path path)
{
	auto obj = Utils::LoadJsonFile(path);
	auto scene = new Scene();
	scene->Unserialize(obj);
	return scene;
}
