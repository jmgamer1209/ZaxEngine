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
    auto golist = obj["list"].as_array(); // obj.  (json::valueto)obj["list"];
    //auto lala = static_cast<Scene*>(malloc(a));
    //cout << lala << "  sdgsg   " << a <<"\n";
    /*Debug::Log(L"什么意思");
    std::cout << obj["list"];*/
    
    for (int i = 0; i < golist.size(); i++)
    {
        auto go = new GameObject();
        go->Unserialize(golist[i].as_object());
        this->AddGameObject(go);
    }

    //Debug::Log(json::serialize());
    //Debug::Log(json::serialize(test));
}