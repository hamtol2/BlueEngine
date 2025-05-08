#include "LightActor.h"
#include "Component/LightComponent.h"

namespace Blue
{
	LightActor::LightActor()
	{
		AddComponent(std::make_shared<LightComponent>());
	}
}