#include "DemoLevel.h"

#include "Actor/Actor.h"
#include "Actor/QuadActor.h"
#include "Actor/SphereActor.h"
#include "Actor/CameraActor.h"
#include "Actor/Soldier/SoldierActor.h"
#include "Actor/James/JamesActor.h"
#include "Actor/SkyboxActor.h"
#include "Actor/FloorActor.h"

#include "Math/Transform.h"
#include "Math/Vector3.h"

namespace Blue
{
	DemoLevel::DemoLevel()
	{
		// 데모 씬(레벨) 구성.

		// 구체 액터 생성.
		std::shared_ptr<SphereActor> sphere = std::make_shared<SphereActor>();
		sphere->transform.position.x = -0.5f;
		sphere->transform.scale = Vector3::One * 0.01f;

		// RenderTexture를 보여주기 위한 Quad 액터 생성.
		std::shared_ptr<QuadActor> quad = std::make_shared<QuadActor>();
		quad->transform.position.x = 1.5f;

		std::shared_ptr<FloorActor> floor = std::make_shared<FloorActor>();

		// 카메라 액터 생성.
		std::shared_ptr<CameraActor> cameraActor = std::make_shared<CameraActor>();
		cameraActor->transform.position.z = -3.0f;

		// 군인 액터 생성.
		std::shared_ptr<SoldierActor> soldier = std::make_shared<SoldierActor>();
		soldier->transform.position.x = -2.0f;
		soldier->transform.position.y = -1.0f;
		soldier->transform.scale = Vector3::One * 0.01f;

		// James 액터 생성.
		std::shared_ptr<JamesActor> james = std::make_shared<JamesActor>();
		james->transform.scale = Vector3::One * 0.01f;
		james->transform.position.y = -1.0f;
		james->transform.position.x = -3.5f;

		// Skybox 액터 생성.
		std::shared_ptr<SkyboxActor> skybox = std::make_shared<SkyboxActor>();
		skybox->transform.scale = Vector3::One * 100.0f;

		// 액터를 레벨에 추가.
		AddActor(sphere);
		AddActor(quad);
		AddActor(floor);
		AddActor(soldier);
		AddActor(james);
		AddActor(skybox);
		AddActor(cameraActor);
	}

	DemoLevel::~DemoLevel()
	{
	}
}