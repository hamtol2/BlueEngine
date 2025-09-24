#include "DemoLevel.h"

#include "Actor/Actor.h"
#include "Actor/QuadActor.h"
#include "Actor/SphereActor.h"
#include "Actor/CameraActor.h"
#include "Actor/LightActor.h"
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
		// ���� ��(����) ����.

		// ��ü ���� ����.
		std::shared_ptr<SphereActor> sphere = std::make_shared<SphereActor>();
		sphere->transform.position.x = -0.5f;
		sphere->transform.scale = Vector3::One * 0.01f;

		// RenderTexture�� �����ֱ� ���� Quad ���� ����.
		std::shared_ptr<QuadActor> quad = std::make_shared<QuadActor>();
		quad->transform.position.x = 1.5f;

		std::shared_ptr<FloorActor> floor = std::make_shared<FloorActor>();

		// ī�޶� ���� ����.
		std::shared_ptr<CameraActor> cameraActor = std::make_shared<CameraActor>();
		cameraActor->transform.position.z = -3.0f;

		// 조명 액터 생성.
		std::shared_ptr<LightActor> light = std::make_shared<LightActor>();
		light->transform.position = Vector3(5.0f, 10.0f, -5.0f);

		//   .
		std::shared_ptr<SoldierActor> soldier = std::make_shared<SoldierActor>();
		soldier->transform.position.x = -2.0f;
		soldier->transform.position.y = -1.0f;
		soldier->transform.scale = Vector3::One * 0.01f;

		// James ���� ����.
		std::shared_ptr<JamesActor> james = std::make_shared<JamesActor>();
		james->transform.scale = Vector3::One * 0.01f;
		james->transform.position.y = -1.0f;
		james->transform.position.x = -3.5f;

		// Skybox ���� ����.
		std::shared_ptr<SkyboxActor> skybox = std::make_shared<SkyboxActor>("Apocalypse");
		skybox->transform.scale = Vector3::One * 100.0f;

		// ���͸� ������ �߰�.
		AddActor(sphere);
		//AddActor(quad);
		AddActor(floor);
		AddActor(soldier);
		AddActor(james);
		AddActor(skybox);
		AddActor(cameraActor);
		AddActor(light);
	}

	DemoLevel::~DemoLevel()
	{
	}
}