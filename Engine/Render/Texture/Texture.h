#pragma once

#include "Core/Type.h"
#include <string>
#include <d3d11.h>
#include <memory>

namespace Blue
{
	// �ؽ�ó ������ �о ������ ����ü.
	struct TextureData
	{
		TextureData() = default;
		~TextureData()
		{
			//// ���� ���ҽ� ����.
			//if (data)
			//{
			//	free(data);
			//	data = nullptr;
			//}

			// DX ���ҽ� ����.
			if (shaderResourceView)
			{
				shaderResourceView->Release();
				shaderResourceView = nullptr;
			}

			if (samplerState)
			{
				samplerState->Release();
				samplerState = nullptr;
			}
		}

		// �ؽ�ó ���� ������.
		int32 width = 0;
		int32 height = 0;
		int32 channelCount = 0;
		void* data = nullptr;

		// DX ���ҽ�.
		ID3D11ShaderResourceView* shaderResourceView = nullptr;
		ID3D11SamplerState* samplerState = nullptr;
	};

	// �ؽ�ó Ŭ����.
	class Texture
	{
		// ���ε��� ���̴� ������.
		enum class BindType
		{
			VertexShader,
			PixelShader
		};

	public:
		Texture();
		Texture(const std::string& name, BindType bindType = BindType::PixelShader);
		virtual ~Texture();

		void Bind(uint32 index = 0);

	protected:

		// �ؽ�ó�� �ε��� �� ����ϴ� �Լ�.
		virtual void LoadTexture(const std::string& name);

		// �ε��� ���� �ؽ�ó �����͸� ����� DX �ؽ�ó ���ҽ��� �����ϴ� �Լ�.
		virtual void CreateSRVAndSampler();

		// 3ä�� �̹����� 4ä�η� ��ȯ�ϴ� �Լ�.
		void ConvertRGBToRGBA(std::unique_ptr<TextureData>& textureData);

	protected:
		// �̹��� �̸�.
		std::string name;

		// ���ε� ���̴� Ÿ��.
		BindType bindType = BindType::PixelShader;

		// �ؽ�ó ������.
		std::unique_ptr<TextureData> textureData;
	};
}