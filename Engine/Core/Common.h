#pragma once

#define WIN_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <comdef.h>

// 오류 메시지 출력 매크로 함수.
#define ShowErrorMessage(message, reason)				\
wchar_t buffer[256] = {};								\
swprintf_s(buffer, 256, TEXT("[File: %s]\n[Line: %d]\n[Function: %s]\n[Message: %s]\n[Reason: %s]"), TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), message, reason );	\
MessageBox(nullptr, buffer, TEXT("Error"), MB_OK);

// D3D 호출 실패 확인 매크로 함수.
#define ThrowIfFailed(result, message)					\
if (FAILED(result))										\
{														\
	_com_error error(result);							\
	ShowErrorMessage(message, error.ErrorMessage());	\
	__debugbreak();										\
}