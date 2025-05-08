#include "Level.h"
#include "Actor/Actor.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"

namespace Blue
{
	Level::Level()
	{
	}

	Level::~Level()
	{
	}

	void Level::BeginPlay()
	{
		if (cameraActor)
		{
			cameraActor->BeginPlay();
		}

		if (lightActor)
		{
			lightActor->BeginPlay();
		}

		for (const auto& actor : actors)
		{
			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		if (cameraActor)
		{
			cameraActor->Tick(deltaTime);
		}

		if (lightActor)
		{
			lightActor->Tick(deltaTime);
		}

		for (const auto& actor : actors)
		{
			actor->Tick(deltaTime);
		}
	}

	void Level::AddActor(std::shared_ptr<Actor> newActor)
	{
		for (auto component : newActor->components)
		{
			// ���� �߰��ϴ� ���Ͱ� ī�޶� ������Ʈ�� �������� Ȯ��.
			// �����ٸ�, ���� ī�޶�� ����.
			std::shared_ptr<CameraComponent> cameraComp = std::dynamic_pointer_cast<CameraComponent>(component);
			if (cameraComp)
			{
				cameraActor = newActor;
				return;
			}

			// ���� �߰��ϴ� ���Ͱ� ����Ʈ ������Ʈ�� �����ٸ�, ����Ʈ ���ͷ� ����.
			std::shared_ptr<LightComponent> lightComp = std::dynamic_pointer_cast<LightComponent>(component);
			if (lightComp)
			{
				lightActor = newActor;
				return;
			}
		}

		actors.emplace_back(newActor);
	}

	std::shared_ptr<Actor> Level::GetActor(int index) const
	{
		// ���� ó��.
		if (index < 0 || index >= (int)actors.size())
		{
			return nullptr;
		}

		return actors[index];
	}

	const uint32 Level::ActorCount() const
	{
		return (uint32)actors.size();
	}

	std::shared_ptr<Actor> Level::GetCamera() const
	{
		return cameraActor;
	}

	std::shared_ptr<Actor> Level::GetLight() const
	{
		return lightActor;
	}
}