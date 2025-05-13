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
		// 장치 생성에 사용하는 옵션.
		uint32 flag = 0u;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// 생성할 라이브러리 버전.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL outFeatureLevel;

		// 장치 생성.
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

		// IDXGIFactory 리소스 생성.
		IDXGIFactory* factory = nullptr;
		//CreateDXGIFactory(__uuidof(factory), reinterpret_cast<void**>(&factory));
		ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)),
			TEXT("Failed to create dxgifactory."));

		// 스왑 체인 정보 구조체.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// 창 모드?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;		// 백버퍼 개수.
		swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 개수.
		swapChainDesc.SampleDesc.Quality = 0; // 멀티 샘플링 수준.
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//D3D_FEATURE_LEVEL targetLevel;

		// 장치 생성.
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

		// SwapChain 생성.
		ThrowIfFailed(factory->CreateSwapChain(
			device,
			&swapChainDesc,
			&swapChain
		), TEXT("Failed to create a swap chain."));

		// 렌더 타겟 뷰 생성.
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

		// 사용한 리소스 해제.
		backbuffer->Release();
		backbuffer = nullptr;

		// 뎁스 스텐실 뷰 생성.
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

		// 2차원 리소스 생성.
		ThrowIfFailed(
			device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer),
			TEXT("Failed to create depth stencil buffer"));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// 뷰 생성.
		ThrowIfFailed(
			device->CreateDepthStencilView(
				depthStencilBuffer,
				&depthStencilViewDesc,
				&depthStencilView),
			TEXT("Failed to create depth stencil view."));

		// 사용한 리소스 해제.
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;

		// 래스터라이저 스테이트 생성.
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

		// 뷰포트(화면).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// 뷰포트 설정.
		context->RSSetViewports(1, &viewport);
	}

	Renderer::~Renderer()
	{
		// DX 리소스 해제.
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
		// 화면 크기 변경 중일 때는 종료.
		if (isResizing)
		{
			return;
		}
		
		// Shadowmap Pass.
		DrawToShadowMap(level);

		// 뷰포트 설정.
		context->RSSetViewports(1, &viewport);

		// Phase-1.
		//DrawToRenderTexturePass(level);

		// Final-Phase.
		DrawFinalPass(level);

		// 버퍼 교환. (EndScene/Present).
		swapChain->Present(1u, 0u);

		static ID3D11ShaderResourceView* nullSRV = nullptr;
		context->PSSetShaderResources(3, 1, &nullSRV);
	}

	void Renderer::SetShadowmap(std::shared_ptr<class Shadowmap>&& shadowmap)
	{
		// 외부에서 생성한 섀도우 맵의 소유권을 Renderer로 이전.
		//this->shadowmap = std::move(shadowmap);
		this->shadowmap = std::move(shadowmap);

		// 테스트를 위한 QuadActor 생성.
		quadActor = std::make_unique<QuadActor>();
		quadActor->transform.position.x = 2.0f;
		quadActor->SetTexture(this->shadowmap);
		quadActor->SetUseRenderTexture(false);
	}

	void Renderer::OnResize(uint32 width, uint32 height)
	{
		// 창 변경으로 인한 리소스 크기 조정.
		if (!device || !context || !swapChain)
		{
			return;
		}

		isResizing = true;

		// context 비우기.
		context->ClearState();
		context->Flush();

		// 렌더타겟 해제.
		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		// 뎁스 스텐실 뷰 해제.
		if (depthStencilView)
		{
			depthStencilView->Release();
			depthStencilView = nullptr;
		}

		// 스왑체인 백버퍼 크기 변경.
		ThrowIfFailed(
			swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0),
			TEXT("Failed to resize swapchain buffer.")
		);

		// 렌더타겟 재생성.
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

		// 뎁스 스텐실 뷰 생성.
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

		// 2차원 리소스 생성.
		ThrowIfFailed(
			device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer),
			TEXT("Failed to create depth stencil buffer"));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// 뷰 생성.
		ThrowIfFailed(
			device->CreateDepthStencilView(
				depthStencilBuffer,
				&depthStencilViewDesc,
				&depthStencilView),
			TEXT("Failed to create depth stencil view."));

		// 사용한 리소스 해제.
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;

		// 뷰포트 업데이트.
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// 뷰포트 설정.
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
		// RTV / SRV 비우기.
		EmptyRTVsAndSRVs();

		// 렌더 타겟 설정.
		context->OMSetRenderTargets(1, renderTargetView, depthStencilView);

		// 지우기(Clear).
		context->ClearRenderTargetView(*renderTargetView, clearColor);
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Renderer::DrawToShadowMap(std::shared_ptr<Level>& level)
	{
		// 섀도우 맵의 뎁스 맵 Clear.
		shadowmap->Clear();

		// 섀도우 맵 바인딩.
		shadowmap->Bind(0);

		// 뎁스 정보 그리기.
		for (uint32 actorIndex = 0; actorIndex < level->ActorCount(); ++actorIndex)
		{
			// 액터 가져오기.
			auto actor = level->GetActor(actorIndex);

			// 렌더 텍스처 사용 여부 확인.
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
				// 스카이 박스는 건너뛰기.
				if (actor->IsSkyBox())
				{
					continue;
				}

				// 뎁스 그리기.
				actor->Draw(true);
			}
		}
		
		// 섀도우맵 셰이더 언바인딩.
		shadowmap->Unbind();
	}

	void Renderer::DrawToRenderTexturePass(std::shared_ptr<Level>& level)
	{
		for (int ix = 0; ix < (int)TextureLoader::Get().renderTextures.size(); ++ix) 
		{
			// 렌더 텍스처 가져오기.
			auto renderTexture = TextureLoader::Get().renderTextures[ix];

			float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			Clear(renderTexture->GetRenderTargetAddress(), color, renderTexture->GetDepthStencilView());

			//// 섀도우 맵 텍스처 바인딩.
			//ID3D11ShaderResourceView* shadowmapSRV = shadowmap->GetShaderResourceView();
			//context->PSSetShaderResources(3, 1, &shadowmapSRV);

			// 그리기.
			// 카메라 바인딩.
			if (level->GetCamera())
			{
				level->GetCamera()->Draw();
			}

			// 라이트 바인딩.
			if (level->GetLight())
			{
				level->GetLight()->Draw();
			}

			for (uint32 actorIndex = 0; actorIndex < level->ActorCount(); ++actorIndex)
			{
				// 액터 가져오기.
				auto actor = level->GetActor(actorIndex);

				// 렌더 텍스처 사용 여부 확인.
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
					// 스카이 박스는 뒷면을 그리도록 설정.
					if (actor->IsSkyBox())
					{
						CullOff();
					}

					actor->Draw();

					// 다시 뒷면을 그리지 않도록 설정.
					CullOn();
				}
			}
		}
	}

	void Renderer::DrawFinalPass(std::shared_ptr<Level>& level)
	{
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		Clear(&renderTargetView, color, depthStencilView);

		// 섀도우 맵 텍스처 바인딩.
		ID3D11ShaderResourceView* shadowmapSRV = shadowmap->GetShaderResourceView();
		context->PSSetShaderResources(3, 1, &shadowmapSRV);

		// Draw.
		// 카메라 바인딩.
		if (level->GetCamera())
		{
			level->GetCamera()->Draw();
		}

		// 라이트 바인딩.
		if (level->GetLight())
		{
			level->GetLight()->Draw();
		}

		for (uint32 ix = 0; ix < level->ActorCount(); ++ix)
		{
			// 액터 가져오기.
			auto actor = level->GetActor(ix);

			// Draw.
			if (actor->IsActive())
			{
				// 스카이 박스는 뒷면을 그리도록 설정.
				if (actor->IsSkyBox())
				{
					CullOff();
				}

				actor->Draw();

				// 다시 뒷면을 그리지 않도록 설정.
				CullOn();
			}
		}

		// @Test: 테스트용 QuadActor 그리기.
		if (quadActor)
		{
			quadActor->Tick(1.0f / 60.0f);
			shadowmap->BindSamplerState();
			quadActor->Draw();
		}
	}
}