#pragma once
#include "Component/Component.h"

class Light: public Component
{
public:
	float color[3] = { 1,1,1 };

private:
	void OnGui() override;
};

