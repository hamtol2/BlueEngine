#include "JamesBodyShader.h"
#include "Render/Texture/Texture.h"
#include "Resource/TextureLoader.h"

namespace Blue
{
	JamesBodyShader::JamesBodyShader()
		: NormalSpecularMappingShader()
	{
		std::weak_ptr<Texture> diffuseMap;
		TextureLoader::Get().Load("James/James_Body_Diffuse.png", diffuseMap);

		std::weak_ptr<Texture> specularMap;
		TextureLoader::Get().Load("James/James_Body_Glossiness.png", specularMap);

		std::weak_ptr<Texture> normalMap;
		TextureLoader::Get().Load("James/James_Body_Normal.png", normalMap);

		SetTexture(ETextureBindType::Diffuse, diffuseMap);
		SetTexture(ETextureBindType::Specular, specularMap);
		SetTexture(ETextureBindType::NormalMap, normalMap);
	}
}