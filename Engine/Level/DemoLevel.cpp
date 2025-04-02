#include "DemoLevel.h"

#include "Actor/Actor.h"
#include "Actor/QuadActor.h"

#include "Math/Transform.h"
#include "Math/Vector3.h"

#include "Render/QuadMesh.h"

#include "Resource/ShaderLoader.h"
#include "Resource/ModelLoader.h"
#include "Resource/TextureLoader.h"

#include "Shader/TextureMappingShader.h"

#include "Component/StaticMeshComponent.h"

namespace Blue
{
	DemoLevel::DemoLevel()
	{
		// 데모 씬(레벨) 구성.

		// 액터 생성.
		std::shared_ptr<QuadActor> actor = std::make_shared<QuadActor>();
		actor->transform.scale = Vector3::One * 0.5f;
		actor->transform.position.x = 0.5f;

		// 액터를 레벨에 추가.
		AddActor(actor);
	}

	DemoLevel::~DemoLevel()
	{
	}
}