#include "Level.h"
#include "Actor/Actor.h"

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
		for (const auto& actor : actors)
		{
			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		for (const auto& actor : actors)
		{
			actor->Tick(deltaTime);
		}
	}

	void Level::AddActor(std::shared_ptr<Actor> newActor)
	{
		actors.emplace_back(newActor);
	}

	std::shared_ptr<Actor> Level::GetActor(int index) const
	{
		// 예외 처리.
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
}