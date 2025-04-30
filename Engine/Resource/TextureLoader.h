#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Core/Type.h"

namespace Blue
{
	class Texture;
	class RenderTexture;
	class TextureLoader
	{
		friend class Renderer;

	public:
		TextureLoader();
		~TextureLoader() = default;

		void Load(const std::string& name, std::weak_ptr<Texture>& outTexture);
		void GetNewRenderTexture(
			std::weak_ptr<RenderTexture>& outTexture,
			uint32 width = 0, uint32 height = 0
		);

		static TextureLoader& Get();

	private:
		static TextureLoader* instance;

		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
		std::vector<std::shared_ptr<RenderTexture>> renderTextures;
	};
}