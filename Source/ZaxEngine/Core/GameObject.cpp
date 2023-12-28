#include "GameObject.h"
#include "imgui/imgui.h"

GameObject::GameObject(const string& name)
{
	this->name = name;
}

void GameObject::AddComponent(Component* component)
{
	component->gameObject = this;
	components.push_back(component);
}