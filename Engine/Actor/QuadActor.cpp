#include "QuadActor.h"

#include "Component/StaticMeshComponent.h"
#include "Render/QuadMesh.h"
#include "Shader/TextureMappingShader.h"
#include "Resource/ShaderLoader.h"

namespace Blue
{
	QuadActor::QuadActor()
	{
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정.
		meshComponent->SetMesh(std::make_shared<QuadMesh>());
		std::weak_ptr<TextureMappingShader> shader;
		if (ShaderLoader::Get().Load<TextureMappingShader>(shader, "T_coord.png"))
		{
			meshComponent->AddShader(shader);
		}
	}
}