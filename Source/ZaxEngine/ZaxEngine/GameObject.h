#pragma once
#include <vector>
#include <typeinfo>
#include <string>
#include "glm/glm.hpp"
using namespace std;

class Component;

class GameObject
{
	vector<Component*> components;

public:
	string name;
	GameObject(const string& name);
    void AddComponent(Component* component);

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

class Component
{
public:
	GameObject* gameObject;
	virtual void Test() {};
};

class Transform: public Component
{
public:
	float position[3] = { 0, 0, 0 };
	float rotation[3] = { 0, 0, 0 };
	float scale[3] = { 1,1,1 };
};