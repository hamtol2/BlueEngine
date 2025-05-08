#pragma once

#include "Actor.h"
#include <string>

namespace Blue
{
	class SkyboxActor : public Actor
	{
	public:
		SkyboxActor();
		SkyboxActor(const std::string& path);
		~SkyboxActor() = default;
	};
}