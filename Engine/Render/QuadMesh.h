#pragma once

#include "Mesh.h"

namespace Blue
{
	// 삼각형 메시 클래스.
	class QuadMesh : public Mesh
	{
	public:
		QuadMesh();

		void Update(float deltaTime);

	private:
		void Rotate(float angle);
	};
}