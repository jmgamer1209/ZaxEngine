#include "Scene.h"

#include <fstream>
#include <iostream>

#include "boost/json.hpp"
#include "Application.h"
#include "Utils.h"
#include "Debug.h"

using namespace boost;

void Scene::AddGameObject(GameObject* go)
{
    list.push_back(go);
}

void Scene::Unserialize(json::object obj)
{
    /*auto obj = Utils::LoadJsonFile(path);
    auto a = sizeof(Scene*);*/
    //auto lala = static_cast<Scene*>(malloc(a));
    //cout << lala << "  sdgsg   " << a <<"\n";
    /*Debug::Log(L"什么意思");
    std::cout << obj["list"];*/
    
    //Debug::Log(json::serialize(obj["list"]));
    //Debug::Log(a);
}