#include "Engine.h"
#include "Window.h"
#include "../Render/Renderer.h"

#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"
#include "Resource/ModelLoader.h"

#include "Level/Level.h"

#include <iostream>

namespace Blue
{
	// 싱글톤 객체 설정.
	Engine* Engine::instance = nullptr;

	Engine::Engine(
		uint32 width,
		uint32 height,
		const std::wstring& title,
		HINSTANCE hInstance)
	{
		// 싱글톤 객체 값 설정.
		instance = this;

		// 창 객체 생성.
		window = std::make_shared<Window>(
			width, height, title, hInstance, WindowProc
		);

		// 셰이더 로더 객체 생성.
		shaderLoader = std::make_unique<ShaderLoader>();

		// 텍스처 로더 객체 생성.
		textureLoader = std::make_unique<TextureLoader>();

		// 모델 로더 객체 생성.
		modelLoader = std::make_unique<ModelLoader>();

		// 렌더러 생성.
		renderer = std::make_shared<Renderer>(
			width, height, window->Handle()
		);
	}

	Engine::~Engine()
	{
	}

	void Engine::Run()
	{
		// 타이머 (틱/델타타임).
		LARGE_INTEGER currentTime;
		LARGE_INTEGER previousTime;
		LARGE_INTEGER frequency;

		// 하드웨어 타이머의 해상도 값(기준 단위).
		QueryPerformanceFrequency(&frequency);

		// 현재 시간.
		QueryPerformanceCounter(&currentTime);
		previousTime = currentTime;

		// 프레임 계산에 사용할 변수.
		float targetFrameRate = 120.0f;
		// 고정 프레임 속도를 사용하기 위한 변수.
		float oneFrameTime = 1.0f / targetFrameRate;

		// 메시지 처리 루프.
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			// 창에 메시지가 들어올때 실행.
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// 메시지 번역.
				TranslateMessage(&msg);

				// 메시지 전달.
				DispatchMessage(&msg);
			}

			// 창에 메시지가 없을 때 다른 작업 처리.
			else
			{
				// 현재 시간 가져오기.
				QueryPerformanceCounter(&currentTime);

				// 프레임 시간 계산.
				float deltaTime = (float)(currentTime.QuadPart - previousTime.QuadPart)
					/ (float)frequency.QuadPart;

				// 프레임 제한.
				if (deltaTime >= oneFrameTime)
				{
					// 출력.
					std::cout << "DeltaTime: " << deltaTime 
						<< " | OneFrameTime: " << oneFrameTime 
						<< " | FPS: " << (int)ceil(1.0f / deltaTime) << "\n";

					// 엔진 루프.
					// 레벨 처리.
					if (mainLevel)
					{
						mainLevel->BeginPlay();
						mainLevel->Tick(deltaTime);
						renderer->Draw(mainLevel);
					}

					// 프레임 시간 업데이트.
					previousTime = currentTime;
				}
			}
		}
	}

	void Engine::SetLevel(std::shared_ptr<Level> newLevel)
	{
		// 메인 레벨 설정.
		mainLevel = newLevel;
	}

	LRESULT Engine::WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		// 메시지 처리.
		switch (message)
		{
			// 창이 삭제되면 실행됨.
		case WM_DESTROY:
			// 이때 프로그램 종료 메시지를 발행.
			PostQuitMessage(0);
			return 0;
		}

		// 기본 메시지 처리.
		return DefWindowProc(handle, message, wparam, lparam);
	}

	Engine& Engine::Get()
	{
		return *instance;
	}

	ID3D11Device& Engine::Device() const
	{
		return *renderer->device;
	}

	ID3D11DeviceContext& Engine::Context() const
	{
		return *renderer->context;
	}
}