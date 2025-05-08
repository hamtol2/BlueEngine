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

		// �ؽ�ó ���� �Լ�.
		void SetTexture(const std::weak_ptr<CubemapTexture>& newTexture);

	private:

		// �ؽ�ó ���ҽ�.
		std::weak_ptr<CubemapTexture> texture;
	};
}