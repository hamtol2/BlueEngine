#include "CameraComponent.h"

#include "Core/Engine.h"
#include "Core/Common.h"

#include "Math/Transform.h"
#include "Actor/Actor.h"

#include "Core/InputController.h"

namespace Blue
{
	CameraComponent::CameraComponent()
	{
		// ����� ������Ʈ �� ���ε�.
		//data.viewMatrix
		//	= Matrix4::Translation(owner->transform.position * -1.0f)
		//	* Matrix4::Transpose(Matrix4::Rotation(owner->transform.rotation));

		// ��� ��ġ.
		data.viewMatrix = Matrix4::Transpose(data.viewMatrix);
		
		// ���� ��� ����.
		data.projectionMatrix = Matrix4::Perspective(
			60.0f,
			(float)Engine::Get().Width(),
			(float)Engine::Get().Height(),
			0.1f,
			10000.0f
		);

		data.projectionMatrix = Matrix4::Transpose(data.projectionMatrix);

		// ������ ��Ƽ� ���� ����.
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(CameraBuffer);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// ���� ������.
		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = &data;

		// ���� ����.
		ID3D11Device& device = Engine::Get().Device();
		ThrowIfFailed(
			device.CreateBuffer(&bufferDesc, &bufferData, &cameraBuffer),
			TEXT("Failed to create camera buffer."));
	}
	
	void CameraComponent::Tick(float deltaTime)
	{
		Component::Tick(deltaTime);

		
	}

	void CameraComponent::Draw()
	{
		Component::Draw();

		// ����� ������Ʈ �� ���ε�.
		data.viewMatrix
			= Matrix4::Translation(owner->transform.position * -1.0f)
			* Matrix4::Transpose(Matrix4::Rotation(owner->transform.rotation));

		// ���� ��� ����.
		data.projectionMatrix = Matrix4::Perspective(
			60.0f,
			(float)Engine::Get().Width(),
			(float)Engine::Get().Height(),
			0.1f,
			10000.0f
		);

		// ī�޶� ��ġ ����.
		data.cameraPosition = owner->transform.position;

		static ID3D11DeviceContext& context = Engine::Get().Context();

		// ��ġ ��� (CPU�� GPU�� ����� �ٷ�� ����� �޶�).
		// ����� ����� ������ ��ķ� ��ȯ�ϱ� ���� ��ġ��� ó��.
		data.viewMatrix = Matrix4::Transpose(data.viewMatrix);
		data.projectionMatrix = Matrix4::Transpose(data.projectionMatrix);

		// ���� ������Ʈ.
		D3D11_MAPPED_SUBRESOURCE resource = {};
		context.Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &data, sizeof(CameraBuffer));
		context.Unmap(cameraBuffer, 0);

		// ���� ���ε�.
		context.VSSetConstantBuffers(1, 1, &cameraBuffer);
	}
}