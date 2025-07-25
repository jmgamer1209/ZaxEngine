#pragma once
#include <vector>
#include <typeinfo>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Component.h"
#include "ZObject.h"
using namespace std;

class Component;

class GameObject:ZObject
{
public:
	vector<Component*> components;
	string name;
	bool isActive = true;

public:
	GameObject();
	GameObject(const string& name);
    void AddComponent(Component* component);
	void Unserialize(json::object obj) override;

	template<typename T>
	T* GetComponent()
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			auto a = typeid(T).name();
			auto b = typeid(*components[i]).name();
			if (a == b)
			{
				return (T*)components[i];
			}
		}
		return nullptr;
	}
};