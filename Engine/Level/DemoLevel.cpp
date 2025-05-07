#include "DemoLevel.h"

#include "Actor/Actor.h"
#include "Actor/QuadActor.h"
#include "Actor/SphereActor.h"
#include "Actor/CameraActor.h"
#include "Actor/SoldierActor.h"

#include "Math/Transform.h"
#include "Math/Vector3.h"

#include "Render/QuadMesh.h"

#include "Resource/ShaderLoader.h"
#include "Resource/ModelLoader.h"
#include "Resource/TextureLoader.h"

#include "Shader/TextureMappingShader.h"

#include "Component/StaticMeshComponent.h"
#include "Component/CameraComponent.h"

namespace Blue
{
	DemoLevel::DemoLevel()
	{
		// 데모 씬(레벨) 구성.

		// 액터 생성.
		std::shared_ptr<SphereActor> sphere = std::make_shared<SphereActor>();
		sphere->transform.position.x = -0.5f;
		sphere->transform.scale = Vector3::One * 0.01f;

		std::shared_ptr<QuadActor> quad = std::make_shared<QuadActor>();
		quad->transform.position.x = 1.0f;

		// 카메라 액터 생성.
		std::shared_ptr<CameraActor> cameraActor = std::make_shared<CameraActor>();
		cameraActor->transform.position.z = -3.0f;

		std::shared_ptr<SoldierActor> soldier = std::make_shared<SoldierActor>();
		soldier->transform.position.x = -2.0f;
		soldier->transform.position.y = -1.0f;
		soldier->transform.scale = Vector3::One * 0.01f;

		// 액터를 레벨에 추가.
		AddActor(sphere);
		AddActor(quad);
		AddActor(soldier);
		AddActor(cameraActor);
	}

	DemoLevel::~DemoLevel()
	{
	}
}