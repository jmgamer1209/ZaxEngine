#include "Component/Collider.h"
#include "Core/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"
#include "Scripting/CS/Bindings/BindingHelper.h"

void Collider::SetShapeScale(Vector3 scale)
{
	
}

void Collider::OnColliderEnter(Collider* other)
{
	for (auto comp:gameObject->components)
	{
		if (comp->onColliderEnterFunc.IsValidate())
		{
			void* args[1];
			args[0] = other->scriptObj.monoObj;
			comp->onColliderEnterFunc.Call(args);
		}
	}
}
