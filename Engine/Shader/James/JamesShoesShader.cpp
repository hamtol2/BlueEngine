#include "JamesShoesShader.h"
#include "Render/Texture.h"
#include "Resource/TextureLoader.h"

namespace Blue
{
	JamesShoesShader::JamesShoesShader()
		: NormalSpecularMappingShader()
	{
		std::weak_ptr<Texture> diffuseMap;
		TextureLoader::Get().Load("James/James_Shoes_Diffuse.png", diffuseMap);

		std::weak_ptr<Texture> specularMap;
		TextureLoader::Get().Load("James/James_Shoes_Glossiness.png", specularMap);

		std::weak_ptr<Texture> normalMap;
		TextureLoader::Get().Load("James/James_Shoes_Normal.png", normalMap);

		SetTexture(ETextureBindType::Diffuse, diffuseMap);
		SetTexture(ETextureBindType::Specular, specularMap);
		SetTexture(ETextureBindType::NormalMap, normalMap);
	}
}