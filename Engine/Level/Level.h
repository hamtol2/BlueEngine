#pragma once

#include <vector>
#include <memory>
#include "Core/Type.h"

namespace Blue
{
	// 월드에 배치된 액터를 관리.
	class Actor;
	class Level
	{
	public:
		Level();
		virtual ~Level();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);

		void AddActor(std::shared_ptr<Actor> newActor);

		// Getter.
		std::shared_ptr<Actor> GetActor(int index) const;
		const uint32 ActorCount() const;

		std::shared_ptr<Actor> GetCamera() const;
		std::shared_ptr<Actor> GetLight() const;

	protected:
		std::vector<std::shared_ptr<Actor>> actors;

		// 메인 카메라 액터.
		std::shared_ptr<Actor> cameraActor;

		// 라이트 액터.
		std::shared_ptr<Actor> lightActor;
	};
}