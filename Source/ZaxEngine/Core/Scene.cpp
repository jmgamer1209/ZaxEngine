#include "Scene.h"

#include <fstream>
#include <iostream>

#include "boost/json.hpp"
#include "Application.h"
#include "Utils.h"

using namespace boost;

void Scene::AddGameObject(GameObject* go)
{
    list.push_back(go);
}

void Scene::DeSerialize(boost::filesystem::path path)
{
    auto obj = Utils::LoadJsonFile(path);
    std::cout << obj["list"];
}