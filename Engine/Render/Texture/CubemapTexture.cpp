#include "CubemapTexture.h"
#include "Core/Common.h"
#include "Core/Engine.h"

namespace Blue
{
#include "Library/stbi_image.h"

	CubemapTexture::CubemapTexture()
	{
	}

	CubemapTexture::CubemapTexture(const std::string& path)
	{
		LoadTexture(path);
	}

	void CubemapTexture::LoadTexture(const std::string& name)
	{
		std::vector<std::string> imageNames;
		imageNames.reserve(6);
		imageNames.emplace_back(std::string("../Assets/Textures/") + name + "/posx.jpg");
		imageNames.emplace_back(std::string("../Assets/Textures/") + name + "/negx.jpg");
		imageNames.emplace_back(std::string("../Assets/Textures/") + name + "/posy.jpg");
		imageNames.emplace_back(std::string("../Assets/Textures/") + name + "/negy.jpg");
		imageNames.emplace_back(std::string("../Assets/Textures/") + name + "/posz.jpg");
		imageNames.emplace_back(std::string("../Assets/Textures/") + name + "/negz.jpg");

		images.reserve(6);

		textureData = std::make_unique<TextureData>();

		int index = 0;
		for (const std::string& imageName : imageNames)
		{
			textureData->data = stbi_load(
				imageName.c_str(),
				&textureData->width,
				&textureData->height,
				&textureData->channelCount,
				0
			);

			if (!textureData->data)
			{
				ThrowIfFailed(E_FAIL, TEXT("Failed to load cubemap image"));
				return;
			}

			if (textureData->channelCount == 3)
			{
				ConvertRGBToRGBA(textureData);
			}

			images.emplace_back((byte*)textureData->data);
			++index;
		}

		CreateSRVAndSampler();

		// 원시 이미지 리소스 해제.
		for (int ix = 0; ix < 6; ++ix)
		{
			free(images[ix]);
			images[ix] = nullptr;
		}
	}

	void CubemapTexture::CreateSRVAndSampler()
	{
		static ID3D11Device& device = Engine::Get().Device();

		// 로드한 이미지 파일 데이터를 기반으로 텍스처 리소스 생성.
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.ArraySize = 6;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		textureDesc.Width = textureData->width;
		textureDesc.Height = textureData->height;

		D3D11_SUBRESOURCE_DATA datas[6] = {};
		for (int ix = 0; ix < 6; ++ix)
		{
			D3D11_SUBRESOURCE_DATA& data = datas[ix];
			data.pSysMem = images[ix];
			data.SysMemPitch = textureData->width * textureData->channelCount;
		}

		ID3D11Texture2D* texture = nullptr;
		ThrowIfFailed(
			device.CreateTexture2D(&textureDesc, datas, &texture),
			TEXT("Error: Failed to create texture2d."));

		// 셰이더 리소스 뷰 속성 설정.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		shaderResourceViewDesc.TextureCube.MipLevels = 1;

		// 셰이더 리소스 뷰 생성(셰이더에 바인딩할 리소스).
		ThrowIfFailed(
			device.CreateShaderResourceView(texture, &shaderResourceViewDesc, &textureData->shaderResourceView),
			TEXT("Error: Failed to create shaderResourceView."));

		// 다 쓴 리소스 해제.
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}

		// 샘플러 속성 설정.
		D3D11_SAMPLER_DESC sampleDesc = {};
		sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.MaxLOD = FLT_MAX;
		sampleDesc.MinLOD = -FLT_MAX;
		sampleDesc.MaxAnisotropy = 3;
		sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

		// 샘플러 생성.
		ThrowIfFailed(
			device.CreateSamplerState(&sampleDesc, &textureData->samplerState),
			TEXT("Error: Failed to create sampler state."));
	}
}