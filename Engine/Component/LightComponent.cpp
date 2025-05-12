#include "LightComponent.h"
#include "Actor/Actor.h"
#include "Math/Transform.h"
#include "Core/Common.h"
#include "Core/Engine.h"

namespace Blue
{
	LightComponent::LightComponent()
	{
		static ID3D11Device& device = Engine::Get().Device();

		// ��� ���� ����.
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = LightData::Stride();
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = &data;

		ThrowIfFailed(
			device.CreateBuffer(&bufferDesc, &bufferData, &dataBuffer),
			TEXT("Failed to create light buffer."));
	}

	LightComponent::~LightComponent()
	{
		if (dataBuffer)
		{
			dataBuffer->Release();
			dataBuffer = nullptr;
		}
	}

	void LightComponent::BeginPlay()
	{
		Component::BeginPlay();

		static ID3D11DeviceContext& context = Engine::Get().Context();

		data.position = owner->transform.position;

		// ����� ������Ʈ �� ���ε�.
		//data.viewMatrix
		//	= Matrix4::Translation(owner->transform.position * -1.0f)
		//	* Matrix4::Transpose(Matrix4::Rotation(owner->transform.rotation));
		data.viewMatrix = Matrix4::LookAt(data.position, Vector3::Zero, Vector3::Up);

		// ���� ��� ����.
		data.projectionMatrix = Matrix4::Perspective(
			90.0f,
			(float)Engine::Get().Width() * 2.0f,
			(float)Engine::Get().Height() * 2.0f,
			0.1f,
			1000.0f
		);

		// ���� ��� ����(���� ����).
		//data.projectionMatrix = Matrix4::Orthographic(lightWidth, lightHeight, 1.0f, 100.0f);

		data.viewMatrix = Matrix4::Transpose(data.viewMatrix);
		data.projectionMatrix = Matrix4::Transpose(data.projectionMatrix);

		// ������ ������Ʈ.
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		context.Map(dataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &data, LightData::Stride());
		context.Unmap(dataBuffer, 0);
	}

	void LightComponent::Tick(float deltaTime)
	{
		Component::Tick(deltaTime);

		static ID3D11DeviceContext& context = Engine::Get().Context();

		data.position = owner->transform.position;

		// ����� ������Ʈ �� ���ε�.
		data.viewMatrix 
			= Matrix4::Translation(owner->transform.position * -1.0f)
			* Matrix4::Transpose(Matrix4::Rotation(owner->transform.rotation));

		//data.viewMatrix = Matrix4::LookAt(data.position, Vector3::Zero, Vector3::Up);

		// ���� ��� ����.
		data.projectionMatrix = Matrix4::Perspective(
			60.0f,
			(float)Engine::Get().Width() * 2.0f,
			(float)Engine::Get().Height() * 2.0f,
			0.1f,
			10000.0f
		);

		// ���� ��� ����(���� ����).
		//data.projectionMatrix = Matrix4::Orthographic(lightWidth, lightHeight, 0.1f, 1000.0f);
		data.viewMatrix = Matrix4::Transpose(data.viewMatrix);
		data.projectionMatrix = Matrix4::Transpose(data.projectionMatrix);

		// ������ ������Ʈ.
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		context.Map(dataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &data, LightData::Stride());
		context.Unmap(dataBuffer, 0);
	}

	void LightComponent::Draw(bool isShadowDraw)
	{
		Component::Draw(isShadowDraw);

		static ID3D11DeviceContext& context = Engine::Get().Context();

		// ����Ʈ ������ ��� ���� ���ε�.
		context.VSSetConstantBuffers(2, 1, &dataBuffer);
		context.PSSetConstantBuffers(2, 1, &dataBuffer);
	}
}