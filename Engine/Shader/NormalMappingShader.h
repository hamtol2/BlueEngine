#pragma once

#include "Shader.h"
#include "Core/Type.h"
#include <memory>
#include <unordered_map>

namespace Blue
{
	class NormalMappingShader : public Shader
	{
	public:
		// ��ǻ��/��ָ� �ؽ�ó ���ε� Ÿ�� ������.
		enum class ETextureBindType : uint32
		{
			Diffuse = 0,
			NormalMap = 1
		};

	public:
		NormalMappingShader();

		virtual void Bind() override;

		// �ؽ�ó ���� �Լ�.
		void SetTexture(
			ETextureBindType bindType,
			const std::weak_ptr<class Texture>& newTexture
		);

	protected:
		// �ؽ�ó ����.
		std::unordered_map<ETextureBindType, std::weak_ptr<class Texture>> textures;
	};
}