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
		// ����ƽ �޽� ������Ʈ ����.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// ���Ϳ� ������Ʈ �߰�.
		AddComponent(meshComponent);

		// ���ҽ� �ε� �� ������Ʈ ����.
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