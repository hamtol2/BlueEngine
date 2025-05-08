#include "NormalMappingShader.h"
#include "Render/Texture/Texture.h"

namespace Blue
{
	NormalMappingShader::NormalMappingShader()
		: Shader(TEXT("NormalMapping"))
	{
	}
	
	void NormalMappingShader::Bind()
	{
		Shader::Bind();

		// �ؽ�ó ���ε�.
		for (const auto& textureRef : textures)
		{
			// ������ �ϰ� �ִ� ���� �ؽ�ó�� ��ȿ���� Ȯ��.
			std::shared_ptr<Texture> texture = textureRef.second.lock();

			// ��ȿ�ϸ� ���ε� ����.
			if (texture)
			{
				// �ε����� ������ ���ε�.
				texture->Bind(
					static_cast<uint32>(textureRef.first)
				);
			}
		}
	}

	void NormalMappingShader::SetTexture(
		ETextureBindType bindType, 
		const std::weak_ptr<class Texture>& newTexture)
	{
		// �ʿ� �߰�.
		textures.insert(std::make_pair(bindType, newTexture));
	}
}