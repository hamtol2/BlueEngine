#include "JamesActor.h"

#include "Core/Common.h"

#include "Component/StaticMeshComponent.h"
#include "Resource/ShaderLoader.h"

#include "Render/James/JamesMesh.h"
#include "Shader/James/JamesBodyShader.h"
#include "Shader/James/JamesShoesShader.h"

namespace Blue
{
	JamesActor::JamesActor()
	{
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정.
		meshComponent->SetMesh(std::make_shared<JamesMesh>());

		std::weak_ptr<JamesBodyShader> bodyShader;
		if (!ShaderLoader::Get().Load<JamesBodyShader>(bodyShader))
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to load james's body shader."));
		}

		std::weak_ptr<JamesShoesShader> shoesShader;
		if (!ShaderLoader::Get().Load<JamesShoesShader>(shoesShader))
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to load james's shoes shader."));
		}

		meshComponent->AddShader(bodyShader);
		meshComponent->AddShader(bodyShader);
		meshComponent->AddShader(shoesShader);
	}
}