#include <Windows.h>

#include "Core/Window.h"
#include "Render/Renderer.h"
#include "Core/Engine.h"

// 창에 관련된 메시지를 처리하는 콜백 함수.
LRESULT CALLBACK WindowProc(
    HWND handle, 
    UINT message, 
    WPARAM wparam, 
    LPARAM lparam)
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

using namespace Blue;

// 창 모드로 할 때 메인 함수(Entry Point).
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    // 엔진 생성 및 실행.
    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);
    engine.Run();

    return 0;
}