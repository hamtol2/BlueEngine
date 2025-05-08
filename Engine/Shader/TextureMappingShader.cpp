#include "TextureMappingShader.h"
#include "Render/Texture/Texture.h"
#include "Resource/TextureLoader.h"
#include "Render/Texture/RenderTexture.h"

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

	void TextureMappingShader::SetTexture(const std::weak_ptr<class Texture>& newTexture)
	{
		// ���� ���� �ؽ�ó�� ���� �ؽ�ó Ÿ������ Ȯ��.
		std::shared_ptr<RenderTexture> renderTexture = std::dynamic_pointer_cast<RenderTexture>(newTexture.lock());

		// ���� �ؽ�ó ��� ���� ����.
		useRenderTexture = renderTexture != nullptr;

		// ���� �ؽ�ó �� ����.
		texture = newTexture;
	}
}