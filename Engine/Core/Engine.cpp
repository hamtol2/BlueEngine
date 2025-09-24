#include "Engine.h"
#include "Window.h"
#include "Render/Renderer.h"
#include "Render/Texture/Shadowmap.h"

#include "Resource/ShaderLoader.h"
#include "Resource/TextureLoader.h"
#include "Resource/ModelLoader.h"

#include "InputController.h"

#include "Level/Level.h"

#include <iostream>

namespace Blue
{
	// �̱��� ��ü ����.
	Engine* Engine::instance = nullptr;

	Engine::Engine(uint32 width, uint32 height, const std::wstring& title, HINSTANCE hInstance)
	{
		// �̱��� ��ü �� ����.
		instance = this;

		// �Է� ������ ��ü ����.
		inputController = std::make_unique<InputController>();

		// â ��ü ����.
		window = std::make_shared<Window>(width, height, title, hInstance, WindowProc);

		// ���̴� �δ� ��ü ����.
		shaderLoader = std::make_unique<ShaderLoader>();

		// �ؽ�ó �δ� ��ü ����.
		textureLoader = std::make_unique<TextureLoader>();

		// �� �δ� ��ü ����.
		modelLoader = std::make_unique<ModelLoader>();

		// 렌더러 생성.
		renderer = std::make_shared<Renderer>(width, height, window->Handle());

		// 쉐도우 맵 생성 및 설정.
		renderer->SetShadowmap(std::make_shared<Shadowmap>(4096, 4096));
	}

	Engine::~Engine()
	{
	}

	void Engine::Run()
	{
		// Ÿ�̸� (ƽ/��ŸŸ��).
		LARGE_INTEGER currentTime;
		LARGE_INTEGER previousTime;
		LARGE_INTEGER frequency;

		// �ϵ���� Ÿ�̸��� �ػ� ��(���� ����).
		QueryPerformanceFrequency(&frequency);

		// ���� �ð�.
		QueryPerformanceCounter(&currentTime);
		previousTime = currentTime;

		// ������ ��꿡 ����� ����.
		float targetFrameRate = 120.0f;
		// ���� ������ �ӵ��� ����ϱ� ���� ����.
		float oneFrameTime = 1.0f / targetFrameRate;

		// �޽��� ó�� ����.
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			// ���� ���� ó��.
			if (isQuit)
			{
				break;
			}

			// â�� �޽����� ���ö� ����.
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// �޽��� ����.
				TranslateMessage(&msg);

				// �޽��� ����.
				DispatchMessage(&msg);
			}

			// â�� �޽����� ���� �� �ٸ� �۾� ó��.
			else
			{
				// ���� �ð� ��������.
				QueryPerformanceCounter(&currentTime);

				// ������ �ð� ���.
				float deltaTime = (float)(currentTime.QuadPart - previousTime.QuadPart)
					/ (float)frequency.QuadPart;

				// ������ ����.
				if (deltaTime >= oneFrameTime)
				{
					// ���.
					//std::cout << "DeltaTime: " << deltaTime 
					//	<< " | OneFrameTime: " << oneFrameTime 
					//	<< " | FPS: " << (int)ceil(1.0f / deltaTime) << "\n";

					// ���ڿ� ����.
					wchar_t stat[512] = { };
					swprintf_s(stat, 512, TEXT("[%s] - [DeltaTime: %f] [FPS: %d]"),
						window->Title().c_str(), deltaTime, (int)ceil(1.0f / deltaTime));

					// â ���� ���.
					SetWindowText(window->Handle(), stat);

					// ���� ����.
					// ���� ó��.
					if (mainLevel)
					{
						mainLevel->BeginPlay();
						mainLevel->Tick(deltaTime);
						renderer->Draw(mainLevel);
					}

					// ������ �ð� ������Ʈ.
					previousTime = currentTime;

					// �Է� �ʱ�ȭ.
					inputController->ResetInputs();
				}
			}
		}
	}

	void Engine::SetLevel(std::shared_ptr<Level> newLevel)
	{
		// ���� ���� ����.
		mainLevel = newLevel;
	}

	LRESULT Engine::WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		// �Է� �����ڰ� �غ� �ȵ����� ����.
		if (!InputController::IsValid())
		{
			return DefWindowProc(handle, message, wparam, lparam);
		}

		// �޽��� ó��.
		switch (message)
		{
			// â�� �����Ǹ� �����.
		case WM_DESTROY:
			// �̶� ���α׷� ���� �޽����� ����.
			//PostQuitMessage(0);
			Engine::Get().Quit();
			break;

		case WM_LBUTTONDOWN:
		{
			InputController::Get().SetButtonUpDown(0, false, true);
		}
		break;

		case WM_LBUTTONUP:
		{
			InputController::Get().SetButtonUpDown(0, true, false);
		}
		break;

		case WM_RBUTTONDOWN:
		{
			InputController::Get().SetButtonUpDown(1, false, true);
		}
		break;

		case WM_RBUTTONUP:
		{
			InputController::Get().SetButtonUpDown(1, true, false);
		}
		break;

		case WM_MBUTTONDOWN:
		{
			InputController::Get().SetButtonUpDown(2, false, true);
		}
		break;

		case WM_MBUTTONUP:
		{
			InputController::Get().SetButtonUpDown(2, true, false);
		}
		break;

		case WM_MOUSEMOVE:
		{
			// ���� ���콺 ������ ��ġ �� ��������.
			int xPosition = LOWORD(lparam);
			int yPosition = HIWORD(lparam);

			InputController::Get().SetMousePosition(xPosition, yPosition);
		}
		break;

		case WM_SIZE:
		{
			if (wparam == SIZE_MINIMIZED)
			{
				break;
			}

			uint32 width = static_cast<uint32>(LOWORD(lparam));
			uint32 height = static_cast<uint32>(HIWORD(lparam));

			// ���� / ���� ũ�� �� ����.
			Engine::Get().OnResize(width, height);
		}
		break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			// MSDN ������ Ȯ���� ����, 30��° ��Ʈ�� KeyUp ���¸� ��Ÿ���ٰ� ����.
			bool isKeyUp = ((lparam & (static_cast<long long>(1) << 30)) != 0);

			// MSDN ������ Ȯ���� ����, 31��° ��Ʈ�� KeyDown ���¸� ��Ÿ���ٰ� ����.
			bool isKeyDown = ((lparam & (static_cast<long long>(1) << 31)) == 0);

			// ������ Ű �Է� ������ ����.
			if (isKeyUp != isKeyDown)
			{
				// ���� Ű ��.
				uint32 vkCode = static_cast<uint32>(wparam);
				InputController::Get().SetKeyUpDown(vkCode, isKeyUp, isKeyDown);
			}
		} break;

		}

		// �⺻ �޽��� ó��.
		return DefWindowProc(handle, message, wparam, lparam);
	}

	Engine& Engine::Get()
	{
		return *instance;
	}

	void Engine::OnResize(uint32 width, uint32 height)
	{
		// ����ó��.
		if (!window)
		{
			return;
		}

		if (!renderer)
		{
			return;
		}

		// ������ Ŭ������ ũ�� ����.
		window->SetWidthHeight(width, height);

		// ��ü â ũ�⿡�� ������ �׷����� ������ ũ��(ClientRect)�� ���ϱ�.
		RECT rect;
		GetClientRect(window->Handle(), &rect);

		uint32 w = (uint32)(rect.right - rect.left);
		uint32 h = (uint32)(rect.bottom - rect.top);

		// �������� ũ�� ���� �Լ� ȣ��.
		renderer->OnResize(w, h);
	}

	void Engine::Quit()
	{
		isQuit = true;
	}

	ID3D11Device& Engine::Device() const
	{
		return *renderer->device;
	}

	ID3D11DeviceContext& Engine::Context() const
	{
		return *renderer->context;
	}

	uint32 Engine::Width() const
	{
		return window->Width();
	}

	uint32 Engine::Height() const
	{
		return window->Height();
	}
}