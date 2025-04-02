#pragma once

// DirectX11 ���.
#include <d3d11.h>
#include <dxgi.h>
#include <memory>

#include "../Core/Type.h"

namespace Blue
{
	// �׷���ī�忡�� ó���ϴ� ��/���ҽ��� �����ϴ� Ŭ����.
	// RHI - Render Hardware Interface.
	class Renderer
	{
		// ���� Ŭ���� friend ����.
		friend class Engine;

	public:
		Renderer(uint32 width, uint32 height, HWND window);
		~Renderer();

		// �׸��� �Լ�.
		void Draw(std::shared_ptr<class Level> level);

	private:
		// ���ҽ�.
		// ��ġ��.
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		IDXGISwapChain* swapChain = nullptr;

		// ����.
		ID3D11RenderTargetView* renderTargetView = nullptr;

		// ����Ʈ.
		D3D11_VIEWPORT viewport;
	};
}