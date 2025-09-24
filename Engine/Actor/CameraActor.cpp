#include "CameraActor.h"
#include "Component/CameraComponent.h"
#include "Core/InputController.h"
#include "Core/Engine.h"

namespace Blue
{
	CameraActor::CameraActor()
	{
		// 카메라 컴포넌트 추가.
		AddComponent(std::make_shared<CameraComponent>());
	}

	void CameraActor::Tick(float deltaTime)
	{
		Actor::Tick(deltaTime);

		// 이동.
		Move(deltaTime);

		// 회전.
		Rotate(deltaTime);
	}

	void CameraActor::Move(float deltaTime)
	{
		// 입력 관리자 포인터 저장.
		static InputController& input = InputController::Get();

		if (input.IsKeyDown(VK_ESCAPE))
		{
			// 팝업 창 띄우기.
			if (MessageBox(nullptr,
				TEXT("Want to Quit?"),
				TEXT("Quit Engine"), MB_YESNO) == IDYES)
			{
				Engine::Get().Quit();
			}
		}

		// 카메라 이동 속력.
		static const float moveSpeed = 1.0f;

		// 카메라 이동 처리.
		if (input.IsKey('A') || input.IsKey(VK_LEFT))
		{
			// 왼쪽 이동.
			//transform.position.x -= 1.0f * deltaTime;
			transform.position -= transform.Right() * moveSpeed * deltaTime;
		}

		if (input.IsKey('D') || input.IsKey(VK_RIGHT))
		{
			// 오른쪽 이동.
			//transform.position.x += 1.0f * deltaTime;
			transform.position += transform.Right() * moveSpeed * deltaTime;
		}

		if (input.IsKey('W') || input.IsKey(VK_UP))
		{
			// 위쪽 이동.
			//transform.position.z += 1.0f * deltaTime;
			transform.position += transform.Forward() * moveSpeed * deltaTime;
		}

		if (input.IsKey('S') || input.IsKey(VK_DOWN))
		{
			// 아래쪽 이동.
			//transform.position.z -= 1.0f * deltaTime;
			transform.position -= transform.Forward() * moveSpeed * deltaTime;
		}

		if (input.IsKey('Q'))
		{
			// 위쪽 이동.
			//transform.position.y -= 1.0f * deltaTime;
			transform.position -= transform.Up() * moveSpeed * deltaTime;
		}

		if (input.IsKey('E'))
		{
			// 아래쪽 이동.
			//transform.position.y += 1.0f * deltaTime;
			transform.position += transform.Up() * moveSpeed * deltaTime;
		}
	}

	void CameraActor::Rotate(float deltaTime)
	{
		// 입력 관리자 포인터 저장.
		static InputController& input = InputController::Get();

		// 마우스 드래그 확인.
		// 0->왼쪽 버튼. 1/2.
		if (input.IsButton(0))
		{
			// 드래그 감도.
			static float sensitivity = 0.05f;

			// 카메라 상하 회전.
			transform.rotation.x += input.GetMouseDeltaY() * sensitivity;

			// 카메라 좌우 회전.
			transform.rotation.y += input.GetMouseDeltaX() * sensitivity;
		}
	}
}