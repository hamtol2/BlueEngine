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

		// 嘉档快 甘 咆胶贸 积己
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

		// 氮胶 胶刨角 轰 积己
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			device.CreateDepthStencilView(shadowMapTexture, &depthStencilViewDesc, &depthStencilView),
			TEXT("Failed to create depth stencil view for shadow map"));

		// 嘉捞歹 府家胶 轰 积己
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = texDesc.MipLevels;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		textureData = std::make_unique<TextureData>();

		ThrowIfFailed(
			device.CreateShaderResourceView(shadowMapTexture, &shaderResourceViewDesc, &textureData->shaderResourceView),
			TEXT("Failed to create shader resource view for shadowmap"));

		// 嘉档快 甘 咆胶贸 积己.
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

		ID3D11Texture2D* renderTargetTexture = nullptr;

		ThrowIfFailed(
			device.CreateTexture2D(&texDesc, nullptr, &renderTargetTexture),
			TEXT("Failed to create texture2d used render target view for shadow map."));

		// 坊歹 鸥百 轰 积己.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			device.CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView),
			TEXT("Failed to create render target view for shadow map."));

		if (renderTargetTexture)
		{
			renderTargetTexture->Release();
			renderTargetTexture = nullptr;
		}

		shadowmapViewport.TopLeftX = 0.0f;
		shadowmapViewport.TopLeftY = 0.0f;
		shadowmapViewport.Width = static_cast<float>(width);
		shadowmapViewport.Height = static_cast<float>(height);
		shadowmapViewport.MinDepth = 0.0f;
		shadowmapViewport.MaxDepth = 1.0f;

		// 绩档快 甘 嘉捞歹 积己.
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
		//static ID3D11RenderTargetView* nullRenderTargetView = nullptr;
		//context.OMSetRenderTargets(1, &nullRenderTargetView, depthStencilView);
		static float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		context.OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		//context.ClearRenderTargetView(renderTargetView, color);
		context.ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		shadowmapShader->Bind();
	}

	void Shadowmap::Unbind()
	{
		shadowmapShader->Unbind();
	}
}