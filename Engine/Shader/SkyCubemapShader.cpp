#include "SkyCubemapShader.h"
#include "Resource/TextureLoader.h"
#include "Render/Texture/CubemapTexture.h"

namespace Blue
{
	SkyCubemapShader::SkyCubemapShader()
		: Shader(TEXT("Cubemap"))
	{
	}

	void SkyCubemapShader::Bind()
	{
		Shader::Bind();

		if (texture.lock())
		{
			texture.lock()->Bind();
		}
	}

	void SkyCubemapShader::SetTexture(const std::weak_ptr<CubemapTexture>& newTexture)
	{
		texture = newTexture;
	}
}