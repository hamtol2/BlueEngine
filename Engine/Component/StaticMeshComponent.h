#pragma once

#include "Component.h"
#include <memory>
#include <vector>

namespace Blue
{
	// 모델링 데이터와 셰이더를 활용해 그리는 컴포넌트.
	class StaticMeshComponent : public Component
	{
	public:
		StaticMeshComponent();
		~StaticMeshComponent();

		// 그리기 함수.
		virtual void Draw(bool isShadowDraw) override;

		// 메시 설정 함수.
		void SetMesh(std::shared_ptr<class Mesh> newMesh);

		// 셰이더 설정 함수.
		void AddShader(std::weak_ptr<class Shader> newShader);

		// 렌더 텍스처 사용 여부 확인 함수.
		bool UseRenderTexture();

	private:
		// 메시(모델링) 데이터.
		std::shared_ptr<class Mesh> mesh;
		// 셰이더 (서브 메시 수 만큼 필요).
		std::vector<std::weak_ptr<class Shader>> shaders;
	};
}