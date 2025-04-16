#pragma once

#include "Math/Vector3.h"
#include "Math/Vector2.h"

namespace Blue
{
	class Vertex
	{
	public:
		Vertex(
			const Vector3& position, 
			const Vector3& color, 
			const Vector2& texCoord,
			const Vector3& normal)
			: position(position), 
			color(color), 
			texCoord(texCoord),
			normal(normal)
		{
		}

		static unsigned int Stride() { return sizeof(Vertex); }

	public:

		// ������ ��ġ.
		Vector3 position;
		
		// ������ ����.
		Vector3 color;

		// ���� ���� �ؽ�ó ��ǥ.
		Vector2 texCoord;

		// ��� (����, Normal) ����.
		Vector3 normal;

		// ź��Ʈ (tangent) ����.
		Vector3 tangent;

		// ����ź��Ʈ (bitangent) ����.
		Vector3 bitangent;
	};
}