#pragma once

#include "Core/Type.h"

#include <d3d11.h>
#include <dxgi.h>
#include <memory>

namespace Blue
{
	// 그래픽카드에서 처리하는 일/리소스를 관리하는 클래스.
	// RHI - Render Hardware Interface.
	class Renderer
	{
		// 엔진 클래스 friend 선언.
		friend class Engine;

	public:
		Renderer(uint32 width, uint32 height, HWND window);
		~Renderer();

		// 그리기 함수.
		void Draw(std::shared_ptr<class Level> level);

		// 섀도우맵 설정 함수.
		void SetShadowmap(std::shared_ptr<class Shadowmap>&& shadowmap);

		// 크기 변경 함수.
		void OnResize(uint32 width, uint32 height);

		// 메시의 뒷면을 안그리도록 설정하는 함수.
		void CullOn();

		// 메시의 뒷면을 그리도록 설정하는 함수.
		void CullOff();

	private:
		// 셰이더에 바인딩된 RTV 및 SRV를 해제하는 함수.
		void EmptyRTVsAndSRVs();

		// RenderTargetView/DepthStencilView를 Clear할 때 사용하는 함수.
		void Clear(ID3D11RenderTargetView** renderTargetView, float* clearColor, ID3D11DepthStencilView* depthStencilView);

		// Shadowmap Pass.
		void DrawToShadowMap(std::shared_ptr<Level>& level);

		// RenderTexture에 Draw하는 Pass.
		void DrawToRenderTexturePass(std::shared_ptr<Level>& level);

		// 최종 RenderTarget에 Draw하는 Pass.
		void DrawFinalPass(std::shared_ptr<Level>& level);

	private:

		// 크기 변경 여부 확인 변수.
		bool isResizing = false;

		// 리소스.
		// 장치류.
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		IDXGISwapChain* swapChain = nullptr;

		// 버퍼.
		ID3D11RenderTargetView* renderTargetView = nullptr;

		// 뎁스 버퍼.
		ID3D11DepthStencilView* depthStencilView = nullptr;

		// 래스터라이저 스테이트.
		ID3D11RasterizerState* cullFrontState = nullptr;
		ID3D11RasterizerState* cullOnRSState = nullptr;

		// 뷰포트.
		D3D11_VIEWPORT viewport;

		// 섀도우 맵.
		std::shared_ptr<class Shadowmap> shadowmap;

		std::unique_ptr<class QuadActor> quadActor;
	};
}