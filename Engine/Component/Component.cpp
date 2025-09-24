#include "Component.h"
#include "Actor/Actor.h"

namespace Blue
{
	Component::Component()
	{
	}

	Component::~Component()
	{
	}

	void Component::BeginPlay()
	{
	}

	void Component::Tick(float deltaTime)
	{
	}
	
	void Component::Draw(bool isShadowDraw)
	{
	}
	
	Actor* Component::GetOwner() const
	{
		return owner;
	}

	void Component::SetOwner(Actor* newOwner)
	{
		owner = newOwner;
	}
}