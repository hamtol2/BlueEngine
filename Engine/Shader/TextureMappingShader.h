#pragma once

#include "Shader.h"
#include <memory>

namespace Blue
{
	class TextureMappingShader : public Shader
	{
	public:
		TextureMappingShader();
		TextureMappingShader(const std::string& textureName);

		virtual void Bind() override;

		// 텍스처 설정 함수.
		void SetTexture(const std::weak_ptr<class Texture>& newTexture);

	private:
		// 텍스처 리소스.
		//std::unique_ptr<class Texture> texture;
		std::weak_ptr<class Texture> texture;
	};
}