#pragma once
#include "GameObject.h"

class Light: public Component
{
public:
	float color[3] = { 1,1,1 };

private:
	void OnGui() override;
};

