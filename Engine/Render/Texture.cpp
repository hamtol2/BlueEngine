#include "Texture.h"
#include "Core/Engine.h"
#include "Core/Common.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Library/stbi_image.h"

namespace Blue
{
	Texture::Texture()
	{
	}

	Texture::Texture(const std::string& name, BindType bindType, uint32 index)
		: name(name), bindType(bindType)/*, index(index)*/
	{
		LoadTexture(name);
	}

	Texture::~Texture()
	{
	}

	void Texture::Bind(uint32 index)
	{
		// 예외 처리.
		if (!textureData)
		{
			return;
		}

		// 컨텍스트 얻기.
		static ID3D11DeviceContext& context = Engine::Get().Context();

		// 바인딩.
		if (bindType == BindType::VertexShader)
		{
			context.VSSetShaderResources(index, 1, &textureData->shaderResourceView);
			context.VSSetSamplers(index, 1, &textureData->samplerState);
		}
		else if (bindType == BindType::PixelShader)
		{
			context.PSSetShaderResources(index, 1, &textureData->shaderResourceView);
			context.PSSetSamplers(index, 1, &textureData->samplerState);
		}
	}

	void Texture::LoadTexture(const std::string& name)
	{
		// 이미지 로드.
		// 경로 설정.
		char path[256] = {};
		sprintf_s(path, 256, "%s", name.c_str());
		char* extension = nullptr;
		char* nameOnly = strtok_s(path, ".", &extension);
		if (extension == nullptr || strcmp(extension, "png") != 0)
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to load texture. only png type is supported."));
			return;
		}

		sprintf_s(path, 256, "../Assets/Textures/%s", name.c_str());

		// 객체 생성.
		textureData = std::make_unique<TextureData>();

		// 이미지 파일 로드.
		textureData->data = stbi_load(
			path,
			&textureData->width,
			&textureData->height,
			&textureData->channelCount,
			0
		);

		// 텍스처 채널 수 확인 (4채널만 지원).
		if (textureData->channelCount == 3)
		{
			// 3채널 이미지를 4채널로 변환.
			ConvertRGBToRGBA(textureData);
		}

		// 예외처리.
		if (!textureData->data)
		{
			std::cout << "Error: Failed to load texture file: " << name << " \n";
			__debugbreak();
			return;
		}

		// DX 리소스 생성.
		// 장치.
		static ID3D11Device& device = Engine::Get().Device();

		// 로드한 이미지 파일 데이터를 기반으로 텍스처 리소스 생성.
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.Width = textureData->width;
		textureDesc.Height = textureData->height;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = textureData->data;
		data.SysMemPitch = textureData->width * textureData->channelCount;

		ID3D11Texture2D* texture = nullptr;
		ThrowIfFailed(
			device.CreateTexture2D(&textureDesc, &data, &texture),
			TEXT("Error: Failed to create texture2d."));

		// 셰이더 리소스 뷰 생성(셰이더에 바인딩할 리소스).
		ThrowIfFailed(
			device.CreateShaderResourceView(texture, nullptr, &textureData->shaderResourceView),
			TEXT("Error: Failed to create shaderResourceView."));

		// 다 쓴 리소스 해제.
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}

		/*
		* D3D11_FILTER Filter;
			D3D11_TEXTURE_ADDRESS_MODE AddressU;
			D3D11_TEXTURE_ADDRESS_MODE AddressV;
			D3D11_TEXTURE_ADDRESS_MODE AddressW;
			FLOAT MipLODBias;
			UINT MaxAnisotropy;
			D3D11_COMPARISON_FUNC ComparisonFunc;
			FLOAT BorderColor[ 4 ];
			FLOAT MinLOD;
			FLOAT MaxLOD;
		*/
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

	void Texture::ConvertRGBToRGBA(std::unique_ptr<TextureData>& textureData)
	{
		// 목표 채널 수(4개);
		uint32 targetChannelCount = 4;
		
		// 픽셀 수.
		uint32 pixelCount = textureData->width * textureData->height;

		// 이미지 버퍼 크기 = 픽셀 수 x 채널 수.
		uint32 size = pixelCount * targetChannelCount;

		// 새로운 이미지 버퍼 생성.
		byte* imageBuffer = new byte[size];

		// 원본 이미지 데이터.
		byte* source = (byte*)(textureData->data);

		// 새로운 이미지 데이터를 생성할 버퍼 포인터.
		byte* dest = imageBuffer;

		// 기존 이미지 데이터 + 알파 채널 추가.
		for (int ix = 0; ix < pixelCount; ++ix)
		{
			dest[0] = source[0];
			dest[1] = source[1];
			dest[2] = source[2];
			dest[3] = 255;

			source += 3;
			dest += 4;
		}

		// 기존에 로드한 이미지 해제.
		if (textureData->data)
		{
			free(textureData->data);
			textureData->data = nullptr;
		}

		// 새로 생성한 이미지 데이터 설정.
		textureData->data = imageBuffer;
		textureData->channelCount = 4;
	}
}