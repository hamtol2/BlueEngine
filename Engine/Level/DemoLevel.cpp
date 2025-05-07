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
		// ���� ��(����) ����.

		// ���� ����.
		std::shared_ptr<SphereActor> actor = std::make_shared<SphereActor>();
		//actor->transform.scale = Vector3::One * 0.5f;
		actor->transform.position.x = -1.0f;

		std::shared_ptr<QuadActor> actor2 = std::make_shared<QuadActor>();
		actor2->transform.position.x = 1.0f;

		// ī�޶� ���� ����.
		//std::shared_ptr<Actor> cameraActor = std::make_shared<Actor>();
		//cameraActor->transform.position.y = -0.5f;
		//cameraActor->AddComponent(std::make_shared<CameraComponent>());
		std::shared_ptr<CameraActor> cameraActor = std::make_shared<CameraActor>();
		cameraActor->transform.position.z = -5.0f;

		std::shared_ptr<SoldierActor> soldier = std::make_shared<SoldierActor>();
		//soldier->transform.scale = Vector3::One * 0.01f;

		//this->cameraActor = cameraActor;

		// ���͸� ������ �߰�.
		//AddActor(actor);
		//AddActor(actor2);
		AddActor(soldier);
		AddActor(cameraActor);
	}

	DemoLevel::~DemoLevel()
	{
	}
}