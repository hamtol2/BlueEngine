#pragma once

#include "Shader/Shader.h"
#include <memory>

namespace Blue
{
	class CubemapTexture;
	class SkyCubemapShader : public Shader
	{
	public:
		SkyCubemapShader();
		~SkyCubemapShader() = default;

		virtual void Bind() override;

		// 텍스처 설정 함수.
		void SetTexture(const std::weak_ptr<CubemapTexture>& newTexture);

	private:

		// 텍스처 리소스.
		std::weak_ptr<CubemapTexture> texture;
	};
}