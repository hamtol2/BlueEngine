#include "Renderer.h"
#include "Core/Common.h"

#include "Level/Level.h"
#include "Actor/Actor.h"
#include "Actor/QuadActor.h"

#include "Math/Transform.h"

#include "Texture/RenderTexture.h"
#include "Resource/TextureLoader.h"
#include "Component/StaticMeshComponent.h"

#include "Texture/Shadowmap.h"

namespace Blue
{
	Renderer::Renderer(uint32 width, uint32 height, HWND window)
	{
		// ��ġ ������ ����ϴ� �ɼ�.
		uint32 flag = 0u;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// ������ ���̺귯�� ����.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL outFeatureLevel;

		// ��ġ ����.
		ThrowIfFailed(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&device,
			&outFeatureLevel,
			&context
		), TEXT("Failed to create devices."));

		// IDXGIFactory ���ҽ� ����.
		IDXGIFactory* factory = nullptr;
		//CreateDXGIFactory(__uuidof(factory), reinterpret_cast<void**>(&factory));
		ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)),
			TEXT("Failed to create dxgifactory."));

		// ���� ü�� ���� ����ü.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// â ���?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;		// ����� ����.
		swapChainDesc.SampleDesc.Count = 1;	// ��Ƽ ���ø� ����.
		swapChainDesc.SampleDesc.Quality = 0; // ��Ƽ ���ø� ����.
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//D3D_FEATURE_LEVEL targetLevel;

		// ��ġ ����.
		//ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		//	nullptr,
		//	D3D_DRIVER_TYPE_HARDWARE,
		//	nullptr,
		//	flag,
		//	featureLevels,
		//	_countof(featureLevels),
		//	D3D11_SDK_VERSION,
		//	&swapChainDesc,
		//	&swapChain,
		//	&device,
		//	nullptr,
		//	&context
		//), TEXT("Failed to create devices"));

		// SwapChain ����.
		ThrowIfFailed(factory->CreateSwapChain(
			device,
			&swapChainDesc,
			&swapChain
		), TEXT("Failed to create a swap chain."));

		// ���� Ÿ�� �� ����.
		ID3D11Texture2D* backbuffer = nullptr;
		//swapChain->GetBuffer(
		//	0,
		//	__uuidof(backbuffer),
		//	reinterpret_cast<void**>(&backbuffer)
		//);

		ThrowIfFailed(swapChain->GetBuffer(
			0,
			IID_PPV_ARGS(&backbuffer)
		), TEXT("Failed to get back buffer"));

		ThrowIfFailed(device->CreateRenderTargetView(
			backbuffer, nullptr, &renderTargetView
		), TEXT("Failed to create render target view"));

		// ����� ���ҽ� ����.
		backbuffer->Release();
		backbuffer = nullptr;

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
			device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer),
			TEXT("Failed to create depth stencil buffer"));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// �� ����.
		ThrowIfFailed(
			device->CreateDepthStencilView(
				depthStencilBuffer,
				&depthStencilViewDesc,
				&depthStencilView),
			TEXT("Failed to create depth stencil view."));

		// ����� ���ҽ� ����.
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;

		// �����Ͷ����� ������Ʈ ����.
		D3D11_RASTERIZER_DESC rasterizerDesc = { };
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.DepthClipEnable = true;

		ThrowIfFailed(
			device->CreateRasterizerState(&rasterizerDesc, &cullOnRSState),
			TEXT("Failed to create cull on rasterizer state."));

		rasterizerDesc.CullMode = D3D11_CULL_FRONT;

		ThrowIfFailed(
			device->CreateRasterizerState(&rasterizerDesc, &cullFrontState),
			TEXT("Failed to create cull off rasterizer state."));

		context->RSSetState(cullOnRSState);

		// ����Ʈ(ȭ��).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// ����Ʈ ����.
		context->RSSetViewports(1, &viewport);
	}

	Renderer::~Renderer()
	{
		// DX ���ҽ� ����.
		if (context)
		{
			context->Release();
			context = nullptr;
		}
		if (swapChain)
		{
			swapChain->Release();
			swapChain = nullptr;
		}

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

		if (cullFrontState)
		{
			cullFrontState->Release();
			cullFrontState = nullptr;
		}

		if (cullOnRSState)
		{
			cullOnRSState->Release();
			cullOnRSState = nullptr;
		}

		if (device)
		{
			device->Release();
			device = nullptr;
		}
	}

	// Ctrl K / Ctrl O.
	void Renderer::Draw(std::shared_ptr<Level> level)
	{
		// ȭ�� ũ�� ���� ���� ���� ����.
		if (isResizing)
		{
			return;
		}
		
		// Shadowmap Pass.
		DrawToShadowMap(level);

		// ����Ʈ ����.
		context->RSSetViewports(1, &viewport);

		// Phase-1.
		//DrawToRenderTexturePass(level);

		// Final-Phase.
		DrawFinalPass(level);

		// ���� ��ȯ. (EndScene/Present).
		swapChain->Present(1u, 0u);

		static ID3D11ShaderResourceView* nullSRV = nullptr;
		context->PSSetShaderResources(3, 1, &nullSRV);
	}

	void Renderer::SetShadowmap(std::shared_ptr<class Shadowmap>&& shadowmap)
	{
		// �ܺο��� ������ ������ ���� �������� Renderer�� ����.
		//this->shadowmap = std::move(shadowmap);
		this->shadowmap = std::move(shadowmap);

		// �׽�Ʈ�� ���� QuadActor ����.
		quadActor = std::make_unique<QuadActor>();
		quadActor->transform.position.x = 2.0f;
		quadActor->SetTexture(this->shadowmap);
		quadActor->SetUseRenderTexture(false);
	}

	void Renderer::OnResize(uint32 width, uint32 height)
	{
		// â �������� ���� ���ҽ� ũ�� ����.
		if (!device || !context || !swapChain)
		{
			return;
		}

		isResizing = true;

		// context ����.
		context->ClearState();
		context->Flush();

		// ����Ÿ�� ����.
		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		// ���� ���ٽ� �� ����.
		if (depthStencilView)
		{
			depthStencilView->Release();
			depthStencilView = nullptr;
		}

		// ����ü�� ����� ũ�� ����.
		ThrowIfFailed(
			swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0),
			TEXT("Failed to resize swapchain buffer.")
		);

		// ����Ÿ�� �����.
		ID3D11Texture2D* backbuffer = nullptr;
		ThrowIfFailed(
			swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)),
			TEXT("Failed to get buffer from swapchain.")
		);

		ThrowIfFailed(
			device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView),
			TEXT("Failed to created render target view.")
		);

		backbuffer->Release();
		backbuffer = nullptr;

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
			device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer),
			TEXT("Failed to create depth stencil buffer"));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// �� ����.
		ThrowIfFailed(
			device->CreateDepthStencilView(
				depthStencilBuffer,
				&depthStencilViewDesc,
				&depthStencilView),
			TEXT("Failed to create depth stencil view."));

		// ����� ���ҽ� ����.
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;

		// ����Ʈ ������Ʈ.
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// ����Ʈ ����.
		context->RSSetViewports(1, &viewport);

		isResizing = false;
	}

	void Renderer::CullOn()
	{
		context->RSSetState(cullOnRSState);
	}

	void Renderer::CullOff()
	{
		context->RSSetState(cullFrontState);
	}

	void Renderer::EmptyRTVsAndSRVs()
	{
		static ID3D11RenderTargetView* nullRTV = nullptr;
		context->OMSetRenderTargets(1, &nullRTV, nullptr);

		static ID3D11ShaderResourceView* nullSRVs = nullptr;
		context->PSSetShaderResources(0, 1, &nullSRVs);
	}

	void Renderer::Clear(ID3D11RenderTargetView** renderTargetView, float* clearColor, ID3D11DepthStencilView* depthStencilView)
	{
		// RTV / SRV ����.
		EmptyRTVsAndSRVs();

		// ���� Ÿ�� ����.
		context->OMSetRenderTargets(1, renderTargetView, depthStencilView);

		// �����(Clear).
		context->ClearRenderTargetView(*renderTargetView, clearColor);
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Renderer::DrawToShadowMap(std::shared_ptr<Level>& level)
	{
		// ������ ���� ���� �� Clear.
		shadowmap->Clear();

		// ������ �� ���ε�.
		shadowmap->Bind(0);

		// ���� ���� �׸���.
		for (uint32 actorIndex = 0; actorIndex < level->ActorCount(); ++actorIndex)
		{
			// ���� ��������.
			auto actor = level->GetActor(actorIndex);

			// ���� �ؽ�ó ��� ���� Ȯ��.
			//auto meshComp = actor->GetComponent<StaticMeshComponent>();
			//if (meshComp && meshComp->UseRenderTexture())
			//{
			//	continue;
			//}
			if (actor->GetUseRenderTexture())
			{
				continue;
			}

			// Draw.
			if (actor->IsActive())
			{
				// ��ī�� �ڽ��� �ǳʶٱ�.
				if (actor->IsSkyBox())
				{
					continue;
				}

				// ���� �׸���.
				actor->Draw(true);
			}
		}
		
		// ������� ���̴� ����ε�.
		shadowmap->Unbind();
	}

	void Renderer::DrawToRenderTexturePass(std::shared_ptr<Level>& level)
	{
		for (int ix = 0; ix < (int)TextureLoader::Get().renderTextures.size(); ++ix) 
		{
			// ���� �ؽ�ó ��������.
			auto renderTexture = TextureLoader::Get().renderTextures[ix];

			float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			Clear(renderTexture->GetRenderTargetAddress(), color, renderTexture->GetDepthStencilView());

			//// ������ �� �ؽ�ó ���ε�.
			//ID3D11ShaderResourceView* shadowmapSRV = shadowmap->GetShaderResourceView();
			//context->PSSetShaderResources(3, 1, &shadowmapSRV);

			// �׸���.
			// ī�޶� ���ε�.
			if (level->GetCamera())
			{
				level->GetCamera()->Draw();
			}

			// ����Ʈ ���ε�.
			if (level->GetLight())
			{
				level->GetLight()->Draw();
			}

			for (uint32 actorIndex = 0; actorIndex < level->ActorCount(); ++actorIndex)
			{
				// ���� ��������.
				auto actor = level->GetActor(actorIndex);

				// ���� �ؽ�ó ��� ���� Ȯ��.
				//auto meshComp = actor->GetComponent<StaticMeshComponent>();
				//if (meshComp && meshComp->UseRenderTexture())
				//{
				//	continue;
				//}
				if (actor->GetUseRenderTexture())
				{
					continue;
				}

				// Draw.
				if (actor->IsActive())
				{
					// ��ī�� �ڽ��� �޸��� �׸����� ����.
					if (actor->IsSkyBox())
					{
						CullOff();
					}

					actor->Draw();

					// �ٽ� �޸��� �׸��� �ʵ��� ����.
					CullOn();
				}
			}
		}
	}

	void Renderer::DrawFinalPass(std::shared_ptr<Level>& level)
	{
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		Clear(&renderTargetView, color, depthStencilView);

		// ������ �� �ؽ�ó ���ε�.
		ID3D11ShaderResourceView* shadowmapSRV = shadowmap->GetShaderResourceView();
		context->PSSetShaderResources(3, 1, &shadowmapSRV);

		// Draw.
		// ī�޶� ���ε�.
		if (level->GetCamera())
		{
			level->GetCamera()->Draw();
		}

		// ����Ʈ ���ε�.
		if (level->GetLight())
		{
			level->GetLight()->Draw();
		}

		for (uint32 ix = 0; ix < level->ActorCount(); ++ix)
		{
			// ���� ��������.
			auto actor = level->GetActor(ix);

			// Draw.
			if (actor->IsActive())
			{
				// ��ī�� �ڽ��� �޸��� �׸����� ����.
				if (actor->IsSkyBox())
				{
					CullOff();
				}

				actor->Draw();

				// �ٽ� �޸��� �׸��� �ʵ��� ����.
				CullOn();
			}
		}

		// @Test: �׽�Ʈ�� QuadActor �׸���.
		if (quadActor)
		{
			quadActor->Tick(1.0f / 60.0f);
			shadowmap->BindSamplerState();
			quadActor->Draw();
		}
	}
}