#include "SphereActor.h"

#include "Component/StaticMeshComponent.h"
#include "Render/SphereMesh.h"

#include "Shader/TextureMappingShader.h"
#include "Shader/NormalMappingShader.h"

#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"
#include "Render/Texture.h"

namespace Blue
{
	SphereActor::SphereActor()
	{
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정. "T_White.png"
		meshComponent->SetMesh(std::make_shared<SphereMesh>());
		std::weak_ptr<NormalMappingShader> shader;
		if (ShaderLoader::Get().Load<NormalMappingShader>(shader))
		{
			meshComponent->AddShader(shader);
		}

		// 텍스처 로드 및 셰이더에 설정.
		std::weak_ptr<Texture> diffuseMap;
		TextureLoader::Get().Load("5k_earth_day_map.png", diffuseMap);
		shader.lock()->SetTexture(
			NormalMappingShader::ETextureBindType::Diffuse,
			diffuseMap
		);

		std::weak_ptr<Texture> normalMap;
		TextureLoader::Get().Load("8k_earth_normal_map.png", normalMap);
		shader.lock()->SetTexture(
			NormalMappingShader::ETextureBindType::NormalMap,
			normalMap
		);
	}

	void SphereActor::Tick(float deltaTime)
	{
		Actor::Tick(deltaTime);

		static const float rotationSpeed = 10.0f;

		// 회전.
		transform.rotation.y += deltaTime * rotationSpeed;
	}
}