#pragma once

#include "NormalMappingShader.h"

namespace Blue
{
	class NormalSpecularMappingShader : public Shader
	{
	public:
		// ��ǻ��/��ָ� �ؽ�ó ���ε� Ÿ�� ������.
		enum class ETextureBindType : uint32
		{
			Diffuse = 0,
			Specular = 1,
			NormalMap = 2
		};

		NormalSpecularMappingShader();

		virtual void Bind() override;

		// �ؽ�ó ���� �Լ�.
		void SetTexture(ETextureBindType bindType, const std::weak_ptr<class Texture>& newTexture);

	protected:

		// �ؽ�ó ����.
		std::unordered_map<ETextureBindType, std::weak_ptr<class Texture>> textures;
	};
}