#pragma once

#include "../Math/Vector3.h"

namespace Blue
{
	class Vertex
	{
	public:
		Vertex(const Vector3& position, const Vector3& color)
			: position(position), color(color)
		{
		}

		static unsigned int Stride() { return sizeof(Vertex); }

	public:
		Vector3 position;
		Vector3 color;
	};
}