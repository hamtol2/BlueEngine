#pragma once

#include "Component.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"

#include <d3d11.h>

namespace Blue
{
	struct LightData
	{
		static uint32 Stride() { return sizeof(LightData); }

		Vector3 position;
		float padding = 0.0f;
		Matrix4 viewMatrix;
		Matrix4 projectionMatrix;
	};

	class LightComponent : public Component
	{
	public:
		LightComponent();
		~LightComponent();

		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;
		virtual void Draw(bool isShadowDraw) override;

	private:
		LightData data;
		float lightWidth = 20.0f;
		float lightHeight = 20.0f;
		ID3D11Buffer* dataBuffer = nullptr;
	};
}