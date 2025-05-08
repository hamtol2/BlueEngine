#pragma once

#include "Texture.h"
#include <vector>

namespace Blue
{
	class CubemapTexture : public Texture
	{
	public:
		CubemapTexture();
		CubemapTexture(const std::string& path);
		~CubemapTexture() = default;

	protected:

		// �ؽ�ó�� �ε��� �� ����ϴ� �Լ�.
		virtual void LoadTexture(const std::string& name) override;

		// �ε��� ���� �ؽ�ó �����͸� ����� DX �ؽ�ó ���ҽ��� �����ϴ� �Լ�.
		virtual void CreateSRVAndSampler() override;

	private:

		std::vector<byte*> images;
	};
}