#include "TextureMappingShader.h"
#include "Render/Texture.h"
#include "Resource/TextureLoader.h"

namespace Blue
{
	TextureMappingShader::TextureMappingShader()
		: Shader(L"TextureMapping")
	{
	}

	TextureMappingShader::TextureMappingShader(const std::string& textureName)
		: Shader(L"TextureMapping")
	{
		// 텍스처 생성.
		//texture = std::make_unique<Texture>(textureName);
		TextureLoader::Get().Load(textureName, texture);
	}

	void TextureMappingShader::Bind()
	{
		Shader::Bind();

		if (texture.lock())
		{
			texture.lock()->Bind();
		}
	}

	void TextureMappingShader::SetTexture(
		const std::weak_ptr<class Texture>& newTexture)
	{
		// 내부 텍스처 값 설정.
		texture = newTexture;
	}
}