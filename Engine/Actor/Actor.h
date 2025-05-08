#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Math/Transform.h" // ���� �� ����� ����.

namespace Blue
{
	// ���� �� �������(DDD).
	class Actor
	{
		friend class Renderer;
		friend class Level;

	public:
		Actor();
		virtual ~Actor();

		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		void Destroy();

		// ������Ʈ �߰� �Լ�.
		void AddComponent(std::shared_ptr<class Component> newComponent);

		// ������Ʈ Get �Լ�.
		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			std::shared_ptr<T> targetComp;
			for (auto& component : components)
			{
				targetComp = std::dynamic_pointer_cast<T>(component);
				if (targetComp)
				{
					return targetComp;
				}
			}

			return nullptr;
		}

		const bool IsActive() const;
		const bool HasInitialized() const;

		const bool IsSkyBox() const { return isSkyBox; }

	public:
		// ������ TRS ������ �����ϴ� Ʈ������.
		Transform transform;

	protected:
		// ������ �̸� (�Ϲ������δ� Hash�� ��ȯ�ؼ� ���).
		std::wstring name = TEXT("Actor");
		// ������ �ʱ�ȭ ����.
		bool hasInitialized = false;
		// ������ Ȱ��ȭ ����.
		bool isActive = true;
		// ���� ����.
		bool hasDestroyed = false;

		// ��ī�̹ڽ� ���� ����.
		bool isSkyBox = false;

		// ������Ʈ �迭.
		std::vector<std::shared_ptr<class Component>> components;
	};
}