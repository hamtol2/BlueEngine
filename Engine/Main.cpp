#include "Core/Engine.h"
#include <iostream>

//#include "Math/Vector2.h"

using namespace Blue;

// 창 모드로 할 때 메인 함수(Entry Point).
//int WINAPI WinMain(
//    _In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd)
//{
//    // 엔진 생성 및 실행.
//    Engine engine(1280, 800, TEXT("Engine Demo"), hInstance);
//    engine.Run();
//
//    return 0;
//}

#include "Core/Common.h"
#include <typeinfo>
#include "Shader/Shader.h"
#include "Shader/TextureMappingShader.h"

template<typename T, typename std::enable_if<std::is_base_of<Shader, T>::value>::type* = nullptr>
void TestClass()
{
	std::boolalpha(std::cout);
	std::cout << typeid(T).name() << "\n";
	std::cout << std::is_base_of<Shader, T>::value << "\n";
}

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
* IA(입력 정리) -> VS ...(Option) -> RS/보간기 -> PS -> OM(출력 조립).
* - VS (정점 변환 Object->World->View->Projection).
* - PS (조명 처리) - PBR / 디퓨즈 / 스페큘러.
* - 엔진 개발자 (렌더러(Renderer) - 개발자).
*/

int main()
{
	Engine engine(1280, 800, TEXT("Engine Demo"), GetModuleHandle(nullptr));
	engine.Run();
}