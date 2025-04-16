#include "QuadActor.h"

#include "Component/StaticMeshComponent.h"
#include "Render/QuadMesh.h"
#include "Render/SphereMesh.h"

#include "Shader/TextureMappingShader.h"
#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"
#include "Render/Texture.h"

namespace Blue
{
	QuadActor::QuadActor()
	{
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정. "T_White.png"
		//meshComponent->SetMesh(std::make_shared<QuadMesh>());
		meshComponent->SetMesh(std::make_shared<SphereMesh>());
		std::weak_ptr<TextureMappingShader> shader;
		if (ShaderLoader::Get().Load<TextureMappingShader>(shader))
		{
			meshComponent->AddShader(shader);
		}

		// 텍스처 로드 및 셰이더에 설정.
		std::weak_ptr<Texture> texture;
		TextureLoader::Get().Load("T_Red.png", texture);
		shader.lock()->SetTexture(texture);
	}
}