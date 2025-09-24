#pragma once

#include "Actor.h"

namespace Blue
{
	class Texture;
	class TextureMappingShader;
	class QuadActor : public Actor
	{
	public:
		QuadActor();
		~QuadActor() = default;

		void SetTexture(const std::weak_ptr<Texture>& newTexture);

		virtual void Tick(float deltaTime) override;

	private:
		std::weak_ptr<TextureMappingShader> shader;
	};
}