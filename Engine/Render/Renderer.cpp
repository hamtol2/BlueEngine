#include "Renderer.h"
#include <vector>
#include <d3dcompiler.h>

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

		// 스왑 체인 정보 구조체.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.Windowed = true;		// 창 모드?.
		swapChainDesc.OutputWindow = window;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;		// 백버퍼 개수.
		swapChainDesc.SampleDesc.Count = 1;	// 멀티 샘플링 개수.
		swapChainDesc.SampleDesc.Quality = 0; // 멀티 샘플링 수준.
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//D3D_FEATURE_LEVEL targetLevel;

		// 장치 생성.
		HRESULT result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain,
			&device,
			nullptr,
			&context
		);

		// 결과 확인.
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to create devices.", "Error", MB_OK);
			__debugbreak();
		}

		// 렌더 타겟 뷰 생성.
		ID3D11Texture2D* backbuffer = nullptr;
		//swapChain->GetBuffer(
		//	0,
		//	__uuidof(backbuffer),
		//	reinterpret_cast<void**>(&backbuffer)
		//);
		result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to get back buffer", "Error", MB_OK);
			__debugbreak();
		}

		result = device->CreateRenderTargetView(
			backbuffer, nullptr, &renderTargetView
		);

		if (FAILED(result))
		{
			MessageBoxA(nullptr, "Failed to create render target view", "Error", MB_OK);
			__debugbreak();
		}

		// 렌더 타겟 뷰 바인딩(연결).
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);

		// 뷰포트(화면).
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		// 뷰포트 설정.
		context->RSSetViewports(1, &viewport);

		// 정점 데이터 생성.
		//std::vector
		// vertex -> vertices.
		float vertices[] =
		{
			0.0f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
		};

		// @Temp: 임시 리소스 생성.
		// 버퍼(Buffer) - 메모리 덩어리.
		D3D11_BUFFER_DESC vertexbufferDesc = {};
		vertexbufferDesc.ByteWidth = sizeof(float) * 3 * 3;
		vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// 정점 데이터.
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices;

		// (정점)버퍼 생성
		result = device->CreateBuffer(
			&vertexbufferDesc, &vertexData, &vertexBuffer);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create vertex buffer",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 인덱스(색인) 버퍼 생성(정점을 조립하는 순서).
		int indices[] = { 0, 1, 2 };

		D3D11_BUFFER_DESC indexbufferDesc = {};
		indexbufferDesc.ByteWidth = sizeof(int) * 3;
		indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// 정점 데이터.
		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indices;

		// 인덱스 버퍼 생성.
		result = device->CreateBuffer(
			&indexbufferDesc, &indexData, &indexBuffer
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create index buffer",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 쉐이더 컴파일.
		ID3DBlob* vertexShaderBuffer = nullptr;
		result = D3DCompileFromFile(
			TEXT("VertexShader.hlsl"),
			nullptr,
			nullptr,
			"main",
			"vs_5_0",
			0, 0,
			&vertexShaderBuffer, nullptr
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to compile vertex shader",
				"Error",
				MB_OK
			);

			__debugbreak();
		}
		
		// 쉐이더 생성.
		result = device->CreateVertexShader(
			vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(),
			nullptr,
			&vertexShader
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create vertex shader",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 입력 레이아웃.
		// 정점 쉐이더에 전달할 정점 데이터가 어떻게 생겼는지 알려줌.
		//LPCSTR SemanticName;
		//UINT SemanticIndex;
		//DXGI_FORMAT Format;
		//UINT InputSlot;
		//UINT AlignedByteOffset;
		//D3D11_INPUT_CLASSIFICATION InputSlotClass;
		//UINT InstanceDataStepRate;
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		result = device->CreateInputLayout(
			inputDesc,
			1,
			vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(),
			&inputlayout
		);

		if (FAILED(result))
		{
			MessageBoxA(
				nullptr,
				"Failed to create input layout",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		// 픽셀 쉐이더 컴파일/생성.
		// 각 리소스 바인딩.
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Draw()
	{
		// 그리기 전 작업 (BeginScene).
		// 지우기(Clear).
		float color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);

		// 드로우(Draw) (Draw).

		// 버퍼 교환. (EndScene/Present).
		swapChain->Present(1u, 0u);
	}
}