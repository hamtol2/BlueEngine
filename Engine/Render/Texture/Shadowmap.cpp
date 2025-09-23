#include "Shadowmap.h"
#include "Core/Common.h"
#include "Core/Engine.h"
#include "Shader/ShadowmapShader.h"

namespace Blue
{
	Shadowmap::Shadowmap(uint32 width, uint32 height)
		: width(width), height(height)
	{
		static ID3D11Device& device = Engine::Get().Device();

		// 깊이 버퍼 텍스처 생성
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		shadowMapTexture = nullptr;
		ThrowIfFailed(
			device.CreateTexture2D(&texDesc, nullptr, &shadowMapTexture),
			TEXT("Failed to create shadowmap texture"));

		// 깊이 스텐실 뷰 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			device.CreateDepthStencilView(shadowMapTexture, &depthStencilViewDesc, &depthStencilView),
			TEXT("Failed to create depth stencil view for shadow map"));

		// 셰이더 리소스 뷰 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = texDesc.MipLevels;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		textureData = std::make_unique<TextureData>();
		textureData->width = width;
		textureData->height = height;

		ThrowIfFailed(
			device.CreateShaderResourceView(shadowMapTexture, &shaderResourceViewDesc, &textureData->shaderResourceView),
			TEXT("Failed to create shader resource view for shadowmap"));

		shadowmapViewport.TopLeftX = 0.0f;
		shadowmapViewport.TopLeftY = 0.0f;
		shadowmapViewport.Width = static_cast<float>(width);
		shadowmapViewport.Height = static_cast<float>(height);
		shadowmapViewport.MinDepth = 0.0f;
		shadowmapViewport.MaxDepth = 1.0f;

		// 쉐도우 맵 셰이더 생성.
		shadowmapShader = std::make_unique<ShadowmapShader>();

		// 샘플러 속성 설정.
		D3D11_SAMPLER_DESC sampleDesc = {};
		sampleDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sampleDesc.MaxLOD = FLT_MAX;
		sampleDesc.MinLOD = -FLT_MAX;
		sampleDesc.MaxAnisotropy = 1;
		sampleDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		sampleDesc.BorderColor[0] = 1.0f;
		sampleDesc.BorderColor[1] = 1.0f;
		sampleDesc.BorderColor[2] = 1.0f;
		sampleDesc.BorderColor[3] = 1.0f;

		// 샘플러 생성.
		ThrowIfFailed(
			device.CreateSamplerState(&sampleDesc, &textureData->samplerState),
			TEXT("Error: Failed to create sampler state."));
	}

	Shadowmap::~Shadowmap()
	{
		if (shadowMapTexture)
		{
			shadowMapTexture->Release();
			shadowMapTexture = nullptr;
		}

		if (depthStencilView)
		{
			depthStencilView->Release();
			depthStencilView = nullptr;
		}

		//if (renderTargetView)
		//{
		//	renderTargetView->Release();
		//	renderTargetView = nullptr;
		//}
	}

	void Shadowmap::Clear()
	{
		static ID3D11DeviceContext& context = Engine::Get().Context();

		context.RSSetViewports(1, &shadowmapViewport);
		context.OMSetRenderTargets(0, nullptr, depthStencilView);
		context.ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Shadowmap::Bind(uint32 index)
	{
		shadowmapShader->Bind();
	}

	void Shadowmap::BindSamplerState()
	{
		if (textureData->samplerState)
		{
			static ID3D11DeviceContext& context = Engine::Get().Context();
			context.PSSetSamplers(0, 1, &textureData->samplerState);
		}
	}

	void Shadowmap::Unbind()
	{
		shadowmapShader->Unbind();
	}
}