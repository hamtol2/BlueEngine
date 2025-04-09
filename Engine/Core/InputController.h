#pragma once

#include "Math/Vector2.h"

// 입력 처리를 담당하는 클래스.
namespace Blue
{
	// 키입력 처리를 위한 구조체.
	struct KeyInputData
	{
		// 입력 관련 변수.
		bool isKeyDown = false;
		bool isKeyUp = false;
		bool isKey = false;

		// 키 값 설정 함수.
		void SetKeyUpDown(bool isKeyUp, bool isKeyDown)
		{
			this->isKeyUp = isKeyUp;
			this->isKeyDown = isKeyDown;

			isKey = isKeyDown && !isKeyUp;
		}
	};
	
	// 마우스 입력 처리를 위한 구조체.
	struct MouseInputData
	{
		// 입력 관련 변수.
		bool isButtonDown = false;
		bool isButtonUp = false;
		bool isButton = false;

		// 키 값 설정 함수.
		void SetButtonUpDown(bool isButtonUp, bool isButtonDown)
		{
			this->isButtonUp = isButtonUp;
			this->isButtonDown = isButtonDown;

			isButton = isButtonDown && !isButtonUp;
		}
	};

	// 입력 관리자 클래스.
	class InputController
	{
	public:

		InputController();
		~InputController();

		// 편의 함수.
		// keyCode -> 버튼 값 (예: 'A').
		bool IsKeyDown(unsigned int keyCode);
		bool IsKeyUp(unsigned int keyCode);
		bool IsKey(unsigned int keyCode);

		// 마우스 입력 관련 함수.
		bool IsButtonDown(unsigned int button);
		bool IsButtonUp(unsigned int button);
		bool IsButton(unsigned int button);

		// 입력 정리 함수.
		void ResetInputs();

		Vector2 GetMousePosition();
		float GetMouseDeltaX();		// 이전 프레임 대비 이동한 거리(X).
		float GetMouseDeltaY();		// 이전 프레임 대비 이동한 거리(Y).

		// 설정 함수.
		void SetKeyUpDown(unsigned int keyCode, bool isKeyUp, bool isKeyDown);
		void SetButtonUpDown(unsigned int button, bool isButtonUp, bool isButtonDown);
		void SetMousePosition(int x, int y);

		// 싱글톤 접근 함수.
		static InputController& Get();

		// 생성 여부(유효성 판단).
		static bool IsValid();

	private:

		// 키/버튼 입력 관련 변수.
		KeyInputData keyInputData[256];
		MouseInputData mouseInputData[3];

		// 마우스 위치 변수.
		Vector2 mousePosition = Vector2::Zero;
		Vector2 mousePreviousPosition = Vector2::Zero;

		// 싱글톤(Singleton) 구현을 위한 변수.
		static InputController* instance;
	};
}