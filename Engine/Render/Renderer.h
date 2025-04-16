#pragma once

// DirectX11 헤더.
#include <d3d11.h>
#include <dxgi.h>
#include <memory>

#include "../Core/Type.h"

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

		// 크기 변경 함수.
		void OnResize(uint32 width, uint32 height);

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

		// 뷰포트.
		D3D11_VIEWPORT viewport;
	};
}