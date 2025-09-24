#pragma once

#include "Shader/NormalSpecularMappingShader.h"

namespace Blue
{
	class SoldierBodyShader : public NormalSpecularMappingShader
	{
	public:
		SoldierBodyShader();
		~SoldierBodyShader() = default;
	};
}