#include "TextureLoader.h"
#include "Render/Texture/Texture.h"
#include "Render/Texture/RenderTexture.h"
#include "Render/Texture/CubemapTexture.h"

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
		// ���ο� �ؽ�ó ����.
		std::shared_ptr<RenderTexture> newTexture = std::make_shared<RenderTexture>(width, height);

		// �迭�� ����.
		renderTextures.emplace_back(newTexture);

		// ��û�� ������ �Ҵ�(����).
		outTexture = newTexture;
	}

	void TextureLoader::LoadCubemap(const std::string& path, std::weak_ptr<CubemapTexture>& outTexture)
	{
		// ���ο� �ؽ�ó ����.
		auto find = cubemapTextures.find(path);
		if (find != cubemapTextures.end())
		{
			outTexture = find->second;
			return;
		}

		std::shared_ptr<CubemapTexture> newTexture = std::make_shared<CubemapTexture>(path);
		cubemapTextures.insert(std::make_pair(path, newTexture));
		outTexture = newTexture;
	}

	TextureLoader& TextureLoader::Get()
	{
		return *instance;
	}
}