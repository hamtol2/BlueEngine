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
		// �ؽ�ó ����.
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
		// ���� �ؽ�ó �� ����.
		texture = newTexture;
	}
}