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

	void TextureMappingShader::SetTexture(const std::weak_ptr<class Texture>& newTexture)
	{
		// 전달 받은 텍스처가 렌더 텍스처 타입인지 확인.
		std::shared_ptr<RenderTexture> renderTexture = std::dynamic_pointer_cast<RenderTexture>(newTexture.lock());

		// 렌더 텍스처 사용 여부 지정.
		useRenderTexture = renderTexture != nullptr;

		// 내부 텍스처 값 설정.
		texture = newTexture;
	}
}