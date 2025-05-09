#pragma once

namespace Blue
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw(bool isShadowDraw);

		class Actor* GetOwner() const;
		void SetOwner(class Actor* newOwner);

	protected:
		class Actor* owner = nullptr;
	};
}