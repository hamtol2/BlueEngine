#pragma once

#include "Component.h"
#include "Math/Matrix4.h"
#include <d3d11.h>

namespace Blue
{
	// 카메라 버퍼 (상수 버퍼).
	struct CameraBuffer
	{
		// 뷰 변환 행렬.
		Matrix4 viewMatrix;
	};

	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		~CameraComponent() = default;
		
		virtual void Tick(float deltaTime) override;
		virtual void Draw() override;

	private:
		// 카메라 속성.
		CameraBuffer data;
		// Dx buffer.
		ID3D11Buffer* cameraBuffer = nullptr;
	};
}