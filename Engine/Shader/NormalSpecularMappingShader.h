#pragma once

#include "NormalMappingShader.h"

namespace Blue
{
	class NormalSpecularMappingShader : public Shader
	{
	public:
		// 디퓨즈/노멀맵 텍스처 바인딩 타입 열거형.
		enum class ETextureBindType : uint32
		{
			Diffuse = 0,
			Specular = 1,
			NormalMap = 2
		};

		NormalSpecularMappingShader();

		virtual void Bind() override;

		// 텍스처 설정 함수.
		void SetTexture(ETextureBindType bindType, const std::weak_ptr<class Texture>& newTexture);

	protected:

		// 텍스처 세장.
		std::unordered_map<ETextureBindType, std::weak_ptr<class Texture>> textures;
	};
}