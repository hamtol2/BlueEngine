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

		// 텍스처를 로드할 때 사용하는 함수.
		virtual void LoadTexture(const std::string& name) override;

		// 로드한 원시 텍스처 데이터를 사용해 DX 텍스처 리소스를 생성하는 함수.
		virtual void CreateSRVAndSampler() override;

	private:

		std::vector<byte*> images;
	};
}