#include "NormalSpecularMappingShader.h"
#include "Render/Texture.h"

namespace Blue
{
	NormalSpecularMappingShader::NormalSpecularMappingShader()
		: Shader(TEXT("NormalSpecularMapping"))
	{
	}

	void NormalSpecularMappingShader::Bind()
	{
		Shader::Bind();

		// 텍스처 바인딩.
		for (const auto& textureRef : textures)
		{
			// 약참조 하고 있는 원본 텍스처가 유효한지 확인.
			std::shared_ptr<Texture> texture = textureRef.second.lock();

			// 유효하면 바인딩 진행.
			if (texture)
			{
				// 인덱스를 지정해 바인딩.
				texture->Bind(static_cast<uint32>(textureRef.first));
			}
		}
	}

	void NormalSpecularMappingShader::SetTexture(ETextureBindType bindType, const std::weak_ptr<class Texture>& newTexture)
	{
		// 맵에 추가.
		textures.insert(std::make_pair(bindType, newTexture));
	}
}