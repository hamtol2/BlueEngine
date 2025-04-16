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

		// �ؽ�ó ���� �Լ�.
		void SetTexture(const std::weak_ptr<class Texture>& newTexture);

	private:
		// �ؽ�ó ���ҽ�.
		//std::unique_ptr<class Texture> texture;
		std::weak_ptr<class Texture> texture;
	};
}