#include "SoldierBodyShader.h"
#include "Render/Texture.h"
#include "Resource/TextureLoader.h"

namespace Blue
{
	SoldierBodyShader::SoldierBodyShader()
	{
		std::weak_ptr<Texture> diffuseMap;
		TextureLoader::Get().Load("Soldier/Soldier_Body_diffuse.png", diffuseMap);

		std::weak_ptr<Texture> normalMap;
		TextureLoader::Get().Load("Soldier/Soldier_Body_normal.png", normalMap);

		SetTexture(ETextureBindType::Diffuse, diffuseMap);
		SetTexture(ETextureBindType::NormalMap, normalMap);
	}
}