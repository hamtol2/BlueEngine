#include "CameraComponent.h"
#include "Core/Engine.h"
#include "Math/Transform.h"
#include "Actor/Actor.h"

namespace Blue
{
	CameraComponent::CameraComponent()
	{
		// 뷰행렬 업데이트 및 바인딩.
		data.viewMatrix
			= Matrix4::Translation(owner->transform.position * -1.0f)
			* Matrix4::Transpose(Matrix4::Rotation(owner->transform.rotation));

		// 데이터 담아서 버퍼 생성.
	}
	
	void CameraComponent::Draw()
	{
		Component::Draw();

		// 뷰행렬 업데이트 및 바인딩.
		data.viewMatrix
			= Matrix4::Translation(owner->transform.position * -1.0f)
			* Matrix4::Transpose(Matrix4::Rotation(owner->transform.rotation));
	}
}