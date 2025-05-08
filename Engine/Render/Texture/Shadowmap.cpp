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

		// ¼Îµµ¿ì ¸Ê ÅØ½ºÃ³ »ý¼º
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		shadowMapTexture = nullptr;
		ThrowIfFailed(
			device.CreateTexture2D(&texDesc, nullptr, &shadowMapTexture),
			TEXT("Failed to create shadowmap texture"));

		// µª½º ½ºÅÙ½Ç ºä »ý¼º
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			device.CreateDepthStencilView(shadowMapTexture, &depthStencilViewDesc, &depthStencilView),
			TEXT("Failed to create depth stencil view for shadow map"));

		// ¼ÎÀÌ´õ ¸®¼Ò½º ºä »ý¼º
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		textureData = std::make_unique<TextureData>();

		ThrowIfFailed(
			device.CreateShaderResourceView(shadowMapTexture, &shaderResourceViewDesc, &textureData->shaderResourceView),
			TEXT("Failed to create shader resource view for shadowmap"));

		shadowmapViewport.TopLeftX = 0.0f;
		shadowmapViewport.TopLeftY = 0.0f;
		shadowmapViewport.Width = static_cast<float>(width);
		shadowmapViewport.Height = static_cast<float>(height);
		shadowmapViewport.MinDepth = 0.0f;
		shadowmapViewport.MaxDepth = 1.0f;

		// ¼¨µµ¿ì ¸Ê ¼ÎÀÌ´õ »ý¼º.
		shadowmapShader = std::make_unique<ShadowmapShader>();
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
	}

	void Shadowmap::Clear()
	{
		static ID3D11DeviceContext& context = Engine::Get().Context();

		context.RSSetViewports(1, &shadowmapViewport);
		static ID3D11RenderTargetView* nullRenderTargetView = nullptr;
		context.OMSetRenderTargets(1, &nullRenderTargetView, depthStencilView);
		context.ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		shadowmapShader->Bind();
	}
}