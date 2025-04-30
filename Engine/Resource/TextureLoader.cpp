#include "TextureLoader.h"
#include "Render/Texture.h"
#include "Render/RenderTexture.h"

namespace Blue
{
	TextureLoader* TextureLoader::instance = nullptr;

	TextureLoader::TextureLoader()
	{
		instance = this;
	}

	void TextureLoader::Load(const std::string& name, std::weak_ptr<Texture>& outTexture)
	{
		auto find = textures.find(name);
		if (find != textures.end())
		{
			outTexture = find->second;
			return;
		}

		std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(name);
		textures.insert(std::make_pair(name, newTexture));
		outTexture = newTexture;
	}

	void TextureLoader::GetNewRenderTexture(
		std::weak_ptr<RenderTexture>& outTexture, uint32 width, uint32 height)
	{
		// 새로운 텍스처 생성.
		std::shared_ptr<RenderTexture> newTexture
			= std::make_shared<RenderTexture>(width, height);

		// 배열에 저장.
		renderTextures.emplace_back(newTexture);

		// 요청한 변수에 할당(설정).
		outTexture = newTexture;
	}

	TextureLoader& TextureLoader::Get()
	{
		return *instance;
	}
}