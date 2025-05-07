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
		// ���� ó��.
		if (!textureData)
		{
			return;
		}

		// ���ؽ�Ʈ ���.
		static ID3D11DeviceContext& context = Engine::Get().Context();

		// ���ε�.
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
		// �̹��� �ε�.
		// ��� ����.
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

		// ��ü ����.
		textureData = std::make_unique<TextureData>();

		// �̹��� ���� �ε�.
		textureData->data = stbi_load(
			path,
			&textureData->width,
			&textureData->height,
			&textureData->channelCount,
			0
		);

		// �ؽ�ó ä�� �� Ȯ�� (4ä�θ� ����).
		if (textureData->channelCount == 3)
		{
			// 3ä�� �̹����� 4ä�η� ��ȯ.
			ConvertRGBToRGBA(textureData);
		}

		// ����ó��.
		if (!textureData->data)
		{
			std::cout << "Error: Failed to load texture file: " << name << " \n";
			__debugbreak();
			return;
		}

		// DX ���ҽ� ����.
		// ��ġ.
		static ID3D11Device& device = Engine::Get().Device();

		// �ε��� �̹��� ���� �����͸� ������� �ؽ�ó ���ҽ� ����.
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

		// ���̴� ���ҽ� �� ����(���̴��� ���ε��� ���ҽ�).
		ThrowIfFailed(
			device.CreateShaderResourceView(texture, nullptr, &textureData->shaderResourceView),
			TEXT("Error: Failed to create shaderResourceView."));

		// �� �� ���ҽ� ����.
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

		// ���÷� ����.
		ThrowIfFailed(
			device.CreateSamplerState(&sampleDesc, &textureData->samplerState),
			TEXT("Error: Failed to create sampler state."));
	}

	void Texture::ConvertRGBToRGBA(std::unique_ptr<TextureData>& textureData)
	{
		// ��ǥ ä�� ��(4��);
		uint32 targetChannelCount = 4;
		
		// �ȼ� ��.
		uint32 pixelCount = textureData->width * textureData->height;

		// �̹��� ���� ũ�� = �ȼ� �� x ä�� ��.
		uint32 size = pixelCount * targetChannelCount;

		// ���ο� �̹��� ���� ����.
		byte* imageBuffer = new byte[size];

		// ���� �̹��� ������.
		byte* source = (byte*)(textureData->data);

		// ���ο� �̹��� �����͸� ������ ���� ������.
		byte* dest = imageBuffer;

		// ���� �̹��� ������ + ���� ä�� �߰�.
		for (int ix = 0; ix < pixelCount; ++ix)
		{
			dest[0] = source[0];
			dest[1] = source[1];
			dest[2] = source[2];
			dest[3] = 255;

			source += 3;
			dest += 4;
		}

		// ������ �ε��� �̹��� ����.
		if (textureData->data)
		{
			free(textureData->data);
			textureData->data = nullptr;
		}

		// ���� ������ �̹��� ������ ����.
		textureData->data = imageBuffer;
		textureData->channelCount = 4;
	}
}