#include "StaticMeshComponent.h"
#include "Render/Mesh.h"
#include "Shader/Shader.h"
#include "Core/Engine.h"
#include "Core/Type.h"

namespace Blue
{
	StaticMeshComponent::StaticMeshComponent()
	{
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
	}

	void StaticMeshComponent::Draw(bool isShadowDraw)
	{
		Component::Draw(isShadowDraw);

		// 서브 메시를 순회하면서 DrawCall.
		uint32 meshCount = mesh->SubMeshCount();

		// 예외처리.
		if (meshCount != (uint32)shaders.size())
		{
			return;
		}

		for (uint32 ix = 0; ix < meshCount; ++ix)
		{
			// 서브 메시 가져오기.
			auto subMesh = mesh->GetSubMesh(ix);

			// 메시가 유효하면 Draw.
			if (subMesh.lock() && shaders[ix].lock())
			{
				// 서브 메시 바인딩.
				subMesh.lock()->Bind();

				// 섀도우를 그리지 않을 때에만 컴포넌트 셰이더 바이딩.
				if (!isShadowDraw)
				{
					// 셰이더 바인딩.
					shaders[ix].lock()->Bind();
				}

				// DrawCall.
				static ID3D11DeviceContext& context = Engine::Get().Context();
				context.DrawIndexed(subMesh.lock()->IndexCount(), 0u, 0u);
			}
		}
	}

	void StaticMeshComponent::SetMesh(std::shared_ptr<Mesh> newMesh)
	{
		mesh = newMesh;
	}

	void StaticMeshComponent::AddShader(std::weak_ptr<Shader> newShader)
	{
		shaders.emplace_back(newShader);
	}
}