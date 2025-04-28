#include "GameObject.h"
#include "imgui/imgui.h"
#include "Debug.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const string& name)
{
	this->name = name;
}

void GameObject::AddComponent(Component* component)
{
	component->gameObject = this;
	components.push_back(component);
}

void GameObject::Unserialize(json::object obj)
{
	//this->name = obj["name"].as_string().c_str();
	//auto components = obj["components"].as_array();
	//for (auto componentdata:components) {
	//	std::string classname = componentdata.as_object()["class"].as_string().c_str();
	//	ZObject::GetObjectFromName(classname)
	//}

}