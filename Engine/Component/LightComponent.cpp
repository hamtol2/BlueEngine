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

		// 버퍼 생성 설정.
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

		// 조명 뷰 매트릭스 설정.
		data.viewMatrix = Matrix4::LookAt(data.position, Vector3::Zero, Vector3::Up);

		// 쉐도우 맵용 직교 투영 설정.
		data.projectionMatrix = Matrix4::Orthographic(lightWidth, lightHeight, 0.1f, 1000.0f);

		data.viewMatrix = Matrix4::Transpose(data.viewMatrix);
		data.projectionMatrix = Matrix4::Transpose(data.projectionMatrix);

		// 버퍼 업데이트.
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

		// 조명 뷰 매트릭스 설정 - 조명 위치에서 원점을 바라보도록.
		data.viewMatrix = Matrix4::LookAt(data.position, Vector3(0.0f, 0.0f, 0.0f), Vector3::Up);

		// 쉐도우 맵용 직교 투영 설정.
		data.projectionMatrix = Matrix4::Orthographic(lightWidth, lightHeight, 0.1f, 1000.0f);

		data.viewMatrix = Matrix4::Transpose(data.viewMatrix);
		data.projectionMatrix = Matrix4::Transpose(data.projectionMatrix);

		// 버퍼 업데이트.
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		context.Map(dataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &data, LightData::Stride());
		context.Unmap(dataBuffer, 0);
	}

	void LightComponent::Draw(bool isShadowDraw)
	{
		Component::Draw(isShadowDraw);

		static ID3D11DeviceContext& context = Engine::Get().Context();

		// 라이트 데이터를 셰이더에 바인딩.
		context.VSSetConstantBuffers(2, 1, &dataBuffer);
		context.PSSetConstantBuffers(2, 1, &dataBuffer);
	}
}