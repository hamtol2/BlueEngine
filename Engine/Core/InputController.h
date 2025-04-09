#pragma once

#include "Math/Vector2.h"

// �Է� ó���� ����ϴ� Ŭ����.
namespace Blue
{
	// Ű�Է� ó���� ���� ����ü.
	struct KeyInputData
	{
		// �Է� ���� ����.
		bool isKeyDown = false;
		bool isKeyUp = false;
		bool isKey = false;

		// Ű �� ���� �Լ�.
		void SetKeyUpDown(bool isKeyUp, bool isKeyDown)
		{
			this->isKeyUp = isKeyUp;
			this->isKeyDown = isKeyDown;

			isKey = isKeyDown && !isKeyUp;
		}
	};
	
	// ���콺 �Է� ó���� ���� ����ü.
	struct MouseInputData
	{
		// �Է� ���� ����.
		bool isButtonDown = false;
		bool isButtonUp = false;
		bool isButton = false;

		// Ű �� ���� �Լ�.
		void SetButtonUpDown(bool isButtonUp, bool isButtonDown)
		{
			this->isButtonUp = isButtonUp;
			this->isButtonDown = isButtonDown;

			isButton = isButtonDown && !isButtonUp;
		}
	};

	// �Է� ������ Ŭ����.
	class InputController
	{
	public:

		InputController();
		~InputController();

		// ���� �Լ�.
		// keyCode -> ��ư �� (��: 'A').
		bool IsKeyDown(unsigned int keyCode);
		bool IsKeyUp(unsigned int keyCode);
		bool IsKey(unsigned int keyCode);

		// ���콺 �Է� ���� �Լ�.
		bool IsButtonDown(unsigned int button);
		bool IsButtonUp(unsigned int button);
		bool IsButton(unsigned int button);

		// �Է� ���� �Լ�.
		void ResetInputs();

		Vector2 GetMousePosition();
		float GetMouseDeltaX();		// ���� ������ ��� �̵��� �Ÿ�(X).
		float GetMouseDeltaY();		// ���� ������ ��� �̵��� �Ÿ�(Y).

		// ���� �Լ�.
		void SetKeyUpDown(unsigned int keyCode, bool isKeyUp, bool isKeyDown);
		void SetButtonUpDown(unsigned int button, bool isButtonUp, bool isButtonDown);
		void SetMousePosition(int x, int y);

		// �̱��� ���� �Լ�.
		static InputController& Get();

		// ���� ����(��ȿ�� �Ǵ�).
		static bool IsValid();

	private:

		// Ű/��ư �Է� ���� ����.
		KeyInputData keyInputData[256];
		MouseInputData mouseInputData[3];

		// ���콺 ��ġ ����.
		Vector2 mousePosition = Vector2::Zero;
		Vector2 mousePreviousPosition = Vector2::Zero;

		// �̱���(Singleton) ������ ���� ����.
		static InputController* instance;
	};
}