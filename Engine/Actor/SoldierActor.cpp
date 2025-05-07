#include "SoldierActor.h"

#include "Core/Common.h"

#include "Component/StaticMeshComponent.h"
#include "Resource/ShaderLoader.h"

#include "Render/SoldierMesh.h"
#include "Shader/SoldierHeadShader.h"
#include "Shader/SoldierBodyShader.h"

namespace Blue
{
	SoldierActor::SoldierActor()
	{
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정.
		meshComponent->SetMesh(std::make_shared<SoldierMesh>());

		std::weak_ptr<SoldierBodyShader> bodyShader;
		if (!ShaderLoader::Get().Load<SoldierBodyShader>(bodyShader))
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to load solidier's body shader."));
		}

		std::weak_ptr<SoldierHeadShader> headShader;
		if (!ShaderLoader::Get().Load<SoldierHeadShader>(headShader))
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to load solidier's head shader."));
		}

		meshComponent->AddShader(bodyShader);
		meshComponent->AddShader(headShader);
		meshComponent->AddShader(bodyShader);
	}
}