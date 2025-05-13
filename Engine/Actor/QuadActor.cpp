#include "QuadActor.h"

#include "Component/StaticMeshComponent.h"
#include "Render/QuadMesh.h"
#include "Render/SphereMesh.h"

#include "Shader/TextureMappingShader.h"
#include "Shader/NormalMappingShader.h"

#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"
#include "Render/Texture/Texture.h"
#include "Render/Texture/RenderTexture.h"

namespace Blue
{
	QuadActor::QuadActor()
	{
		// ����ƽ �޽� ������Ʈ ����.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// ���Ϳ� ������Ʈ �߰�.
		AddComponent(meshComponent);

		// ���ҽ� �ε� �� ������Ʈ ����. "T_White.png"
		meshComponent->SetMesh(std::make_shared<QuadMesh>());
		std::weak_ptr<TextureMappingShader> shader;
		if (ShaderLoader::Get().Load<TextureMappingShader>(shader))
		{
			meshComponent->AddShader(shader);
			this->shader = shader;
		}

		//// �ؽ�ó �ε� �� ���̴��� ����.
		//std::weak_ptr<RenderTexture> renderTexture;
		//TextureLoader::Get().GetNewRenderTexture(
		//	renderTexture, 1280, 800
		//);

		//shader.lock()->SetTexture(renderTexture);
		//SetUseRenderTexture(true);
	}

	void QuadActor::SetTexture(const std::weak_ptr<Texture>& newTexture)
	{
		shader.lock()->SetTexture(newTexture);
	}

	void QuadActor::Tick(float deltaTime)
	{
		Actor::Tick(deltaTime);

		static const float rotationSpeed = 10.0f;

		// ȸ��.
		//transform.rotation.y += deltaTime * rotationSpeed;
	}
}