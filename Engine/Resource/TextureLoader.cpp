#include "TextureLoader.h"
#include "Render/Texture.h"

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

	TextureLoader& TextureLoader::Get()
	{
		return *instance;
	}
}