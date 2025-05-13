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
		// 스태틱 메시 컴포넌트 생성.
		std::shared_ptr<StaticMeshComponent> meshComponent
			= std::make_shared<StaticMeshComponent>();

		// 액터에 컴포넌트 추가.
		AddComponent(meshComponent);

		// 리소스 로드 및 컴포넌트 설정. "T_White.png"
		meshComponent->SetMesh(std::make_shared<QuadMesh>());
		std::weak_ptr<TextureMappingShader> shader;
		if (ShaderLoader::Get().Load<TextureMappingShader>(shader))
		{
			meshComponent->AddShader(shader);
			this->shader = shader;
		}

		//// 텍스처 로드 및 셰이더에 설정.
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

		// 회전.
		//transform.rotation.y += deltaTime * rotationSpeed;
	}
}