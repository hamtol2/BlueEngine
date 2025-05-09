#pragma once

#include "Component.h"
#include <memory>
#include <vector>

namespace Blue
{
	// �𵨸� �����Ϳ� ���̴��� Ȱ���� �׸��� ������Ʈ.
	class StaticMeshComponent : public Component
	{
	public:
		StaticMeshComponent();
		~StaticMeshComponent();

		// �׸��� �Լ�.
		virtual void Draw(bool isShadowDraw) override;

		// �޽� ���� �Լ�.
		void SetMesh(std::shared_ptr<class Mesh> newMesh);

		// ���̴� ���� �Լ�.
		void AddShader(std::weak_ptr<class Shader> newShader);

		// ���� �ؽ�ó ��� ���� Ȯ�� �Լ�.
		bool UseRenderTexture();

	private:
		// �޽�(�𵨸�) ������.
		std::shared_ptr<class Mesh> mesh;
		// ���̴� (���� �޽� �� ��ŭ �ʿ�).
		std::vector<std::weak_ptr<class Shader>> shaders;
	};
}