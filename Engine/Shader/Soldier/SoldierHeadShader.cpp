#include "SoldierHeadShader.h"
#include "Render/Texture/Texture.h"
#include "Resource/TextureLoader.h"

namespace Blue
{
	SoldierHeadShader::SoldierHeadShader()
		: NormalSpecularMappingShader()
	{
		std::weak_ptr<Texture> diffuseMap;
		TextureLoader::Get().Load("Soldier/Soldier_head_diffuse.png", diffuseMap);

		std::weak_ptr<Texture> specularMap;
		TextureLoader::Get().Load("Soldier/Soldier_head_specular.png", specularMap);

		std::weak_ptr<Texture> normalMap;
		TextureLoader::Get().Load("Soldier/Soldier_head_normal.png", normalMap);

		SetTexture(ETextureBindType::Diffuse, diffuseMap);
		SetTexture(ETextureBindType::Specular, specularMap);
		SetTexture(ETextureBindType::NormalMap, normalMap);
	}
}