#pragma once

#include "Shader/NormalSpecularMappingShader.h"

namespace Blue
{
	class JamesBodyShader : public NormalSpecularMappingShader
	{
	public:
		JamesBodyShader();
		~JamesBodyShader() = default;
	};
}