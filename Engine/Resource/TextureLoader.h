#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace Blue
{
	class Texture;
	class TextureLoader
	{
	public:
		TextureLoader();
		~TextureLoader() = default;

		void Load(const std::string& name, std::weak_ptr<Texture>& outTexture);

		static TextureLoader& Get();

	private:
		static TextureLoader* instance;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}