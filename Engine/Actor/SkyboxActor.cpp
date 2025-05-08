#include "SkyboxActor.h"
#include "Core/Common.h"

#include "Component/StaticMeshComponent.h"
#include "Render/CubeMesh.h"
#include "Render/Texture/CubemapTexture.h"
#include "Shader/SkyCubemapShader.h"

#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"

namespace Blue
{
	SkyboxActor::SkyboxActor()
	{
		// ��ī�� �ڽ��� ����.
		isSkyBox = true;

		// ����ƽ �޽� ������Ʈ ����.
		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();

		// ���Ϳ� ������Ʈ �߰�.
		AddComponent(meshComponent);

		// ���ҽ� �ε� �� ������Ʈ ����.
		meshComponent->SetMesh(std::make_shared<CubeMesh>());

		std::weak_ptr<SkyCubemapShader> cubemapShader;
		if (!ShaderLoader::Get().Load<SkyCubemapShader>(cubemapShader))
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to load james's body shader."));
		}

		std::weak_ptr<CubemapTexture> cubeMap;
		TextureLoader::Get().LoadCubemap("Sanfrancisco4", cubeMap);
		cubemapShader.lock()->SetTexture(cubeMap);

		meshComponent->AddShader(cubemapShader);
	}
}