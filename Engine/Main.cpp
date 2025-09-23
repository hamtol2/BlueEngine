#include "Core/Engine.h"
#include "Level/DemoLevel.h"
#include <iostream>

using namespace Blue;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// â ���� �� �� ���� �Լ�(Entry Point).
//int WINAPI WinMain(
//    _In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd)
//{
//    // ���� ���� �� ����.
//    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);
//    engine.Run();
//
//    return 0;
//}

/*
* Todo List.
* - Input Controller (Window Message Process).
* - Actor(Entity/GameObject) / Component / Scene(Level).
* - FBX Loader (Assimp/FBX SDK -> Submesh).
* - Camera ( View / Projection(Perspective/Othographic) ).
*   - Screen Coordinate System (NDC - Normalized Device Coordinates).
* - Shade (Shader) (Light / Brightness / Darkness ).
*   - Ambient(Static/GI) / Diffuse (Lambert .../Roughness) / Specular (Highlight, Phong/Blinn-Phong / Metalic).
* - Normal Mapping.
* - Render Target (Draw to texture).
* 
* - Shadow Mapping (Shadow Map).
* - Environment Mapping (Sky Sphere(Box)).
* - Study (Tone Mapping).
* 
* - Skeletal Mesh (Skinned Mesh) - Skeleton.
*   - Skeletal Mesh Animation.
* - Toy Project...
* 
* - Study (Physics).
*/

/*
* IA(�Է� ����) -> VS ...(Option) -> RS/������ -> PS -> OM(��� ����).
* - VS (���� ��ȯ Object->World->View->Projection).
* - PS (���� ó��) - PBR / ��ǻ�� / ����ŧ��.
* - ���� ������ (������(Renderer) - ������).
*/

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.SetLevel(std::make_shared<DemoLevel>());
	engine.Run();
}