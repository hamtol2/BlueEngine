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
		// ����ƽ �޽� ������Ʈ ����.
		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();

		// ���Ϳ� ������Ʈ �߰�.
		AddComponent(meshComponent);

		// ���ҽ� �ε� �� ������Ʈ ����.
		meshComponent->SetMesh(std::make_shared<QuadMesh>());

		std::weak_ptr<FloorShader> shader;
		if (ShaderLoader::Get().Load<FloorShader>(shader))
		{
			meshComponent->AddShader(shader);
		}

		// �ؽ�ó �ε� �� ���̴��� ����.
		std::weak_ptr<Texture> texture;
		TextureLoader::Get().Load("T_White.png", texture);

		shader.lock()->SetTexture(texture);

		// Ʈ������ ����.
		transform.position.y = -1.0f;
		transform.rotation.x = 90.0f;
		transform.scale = Vector3::One * 10.0f;
	}
}