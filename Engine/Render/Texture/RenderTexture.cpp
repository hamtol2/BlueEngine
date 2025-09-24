#include "RenderTexture.h"
#include "Core/Engine.h"
#include "Core/Common.h"

namespace Blue
{
	RenderTexture::RenderTexture(uint32 width, uint32 height)
	{
		// RTV / SRV / DSV.
		static ID3D11Device& device = Engine::Get().Device();

		// �ʺ�/���� �� ����.
		width = width == 0 ? Engine::Get().Width() : width;
		height = height == 0 ? Engine::Get().Height() : height;

		// textureData ����.
		textureData = std::make_unique<TextureData>();
		textureData->width = width;
		textureData->height = height;

		// RTV ����.
		// �ܰ�1 -> RTV ������ ����� �ؽ�ó ����.
		// �ܰ�2 -> ������ �ؽ�ó�� �ɼ��� �߰��� RTV ����.

		// �ܰ�1.
		ID3D11Texture2D* renderTargetResource = nullptr;
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags =
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		// ����.
		ThrowIfFailed(
			device.CreateTexture2D(&textureDesc, nullptr, &renderTargetResource),
			TEXT("Failed to create texture2D for render texture."));

		// ���� Ÿ�� �� ����.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = { };
		renderTargetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		ThrowIfFailed(device.CreateRenderTargetView(
			renderTargetResource,
			&renderTargetViewDesc,
			&renderTargetView
		), TEXT("Failed to create render target view for render texture."));

		// SRV ����.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		ThrowIfFailed(device.CreateShaderResourceView(
			renderTargetResource,
			&shaderResourceViewDesc,
			&textureData->shaderResourceView
		), TEXT("Failed to create shader resource view for render texture."));

		// ���ҽ� ����.
		renderTargetResource->Release();
		renderTargetResource = nullptr;

		// ���÷� ������Ʈ ����.
		D3D11_SAMPLER_DESC sampleDesc = {};
		sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampleDesc.MaxLOD = FLT_MAX;
		sampleDesc.MinLOD = -FLT_MAX;
		sampleDesc.MaxAnisotropy = 3;
		sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

		ThrowIfFailed(
			device.CreateSamplerState(&sampleDesc, &textureData->samplerState),
			TEXT("Failed to create sampler state for render texture"));

		// ���� ���ٽ� �� ����.
		ID3D11Texture2D* depthStencilBuffer = nullptr;
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		// 2���� ���ҽ� ����.
		ThrowIfFailed(
			device.CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer),
			TEXT("Failed to create depth stencil buffer"));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// �� ����.
		ThrowIfFailed(
			device.CreateDepthStencilView(
				depthStencilBuffer,
				&depthStencilViewDesc,
				&depthStencilView),
			TEXT("Failed to create depth stencil view."));

		// ����� ���ҽ� ����.
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}

	RenderTexture::~RenderTexture()
	{
		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		if (depthStencilView)
		{
			depthStencilView->Release();
			depthStencilView = nullptr;
		}
	}
}