#include "Actor.h"
#include "Component/Component.h"

namespace Blue
{
	Actor::Actor()
	{
	}

	Actor::~Actor()
	{
	}
	
	void Actor::BeginPlay()
	{
		// 예외 처리.
		if (hasInitialized)
		{
			return;
		}

		// 초기화 설정.
		hasInitialized = true;

		// 컴포넌트 함수 호출.
		for (const auto& component : components)
		{
			component->BeginPlay();
		}
	}
	
	void Actor::Tick(float deltaTime)
	{
		// 예외 처리.
		if (!IsActive())
		{
			return;
		}

		// 트랜스폼 업데이트.
		transform.Tick();

		// 컴포넌트 함수 호출.
		for (const auto& component : components)
		{
			component->Tick(deltaTime);
		}
	}
	
	void Actor::Draw(bool isShadowDraw)
	{
		// 예외 처리.
		if (!IsActive())
		{
			return;
		}

		// 트랜스폼 바인딩 (셰이더에 데이터 전달).
		transform.Bind();

		// 컴포넌트 함수 호출.
		for (const auto& component : components)
		{
			component->Draw(isShadowDraw);
		}
	}

	void Actor::Destroy()
	{
		hasDestroyed = true;
	}
	
	void Actor::AddComponent(std::shared_ptr<Component> newComponent)
	{
		// 컴포넌트 배열에 새로운 컴포넌트 추가.
		components.emplace_back(newComponent);

		// 컴포넌트의 소유 설정.
		newComponent->SetOwner(this);
	}

	void Actor::SetUseRenderTexture(bool newUseRenderTexture)
	{
		useRenderTexture = newUseRenderTexture;
	}

	const bool Actor::IsActive() const
	{
		return isActive && !hasDestroyed;
	}
	
	const bool Actor::HasInitialized() const
	{
		return hasInitialized;
	}
}