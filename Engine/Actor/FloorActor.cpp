#include "FloorActor.h"

#include "Component/StaticMeshComponent.h"
#include "Shader/FloorShader.h"

#include "Render/QuadMesh.h"
#include "Render/Texture/Texture.h"

#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"

#include "Math/Vector3.h"

namespace Blue
{
	FloorActor::FloorActor()
	{
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정.
		meshComponent->SetMesh(std::make_shared<QuadMesh>());

		std::weak_ptr<FloorShader> shader;
		if (ShaderLoader::Get().Load<FloorShader>(shader))
		{
			meshComponent->AddShader(shader);
		}

		// 텍스처 로드 및 셰이더에 설정.
		std::weak_ptr<Texture> texture;
		TextureLoader::Get().Load("T_White.png", texture);

		shader.lock()->SetTexture(texture);

		// 트랜스폼 설정.
		transform.position.y = -1.0f;
		transform.rotation.x = 90.0f;
		transform.scale = Vector3::One * 10.0f;
	}
}