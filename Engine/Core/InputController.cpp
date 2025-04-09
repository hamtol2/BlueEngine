#include "InputController.h"

namespace Blue
{
	// static 변수 정의.
	InputController* InputController::instance = nullptr;

	InputController::InputController()
	{
	}
	InputController::~InputController()
	{
	}
	bool InputController::IsKeyDown(unsigned int keyCode)
	{
		return false;
	}
	bool InputController::IsKeyUp(unsigned int keyCode)
	{
		return false;
	}
	bool InputController::IsKey(unsigned int keyCode)
	{
		return false;
	}
	bool InputController::IsButtonDown(unsigned int button)
	{
		return false;
	}
	bool InputController::IsButtonUp(unsigned int button)
	{
		return false;
	}
	bool InputController::IsButton(unsigned int button)
	{
		return false;
	}
	Vector2 InputController::GetMousePosition()
	{
		return Vector2();
	}
	float InputController::GetMouseDeltaX()
	{
		return 0.0f;
	}
	float InputController::GetMouseDeltaY()
	{
		return 0.0f;
	}
	void InputController::SetKeyUpDown(unsigned int keyCode, bool isKeyUp, bool isKeyDown)
	{
	}
	void InputController::SetButtonUpDown(unsigned int button, bool isButtonUp, bool isButtonDown)
	{
	}
	void InputController::SetMousePosition(int x, int y)
	{
	}
	InputController& InputController::Get()
	{
		// TODO: insert return statement here
	}
}