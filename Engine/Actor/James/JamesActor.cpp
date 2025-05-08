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
		// ����ƽ �޽� ������Ʈ ����.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// ���Ϳ� ������Ʈ �߰�.
		AddComponent(meshComponent);

		// ���ҽ� �ε� �� ������Ʈ ����.
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