#include "Matrix4.h"

namespace Blue
{
	Matrix4 Matrix4::Identity = Matrix4();

	Matrix4::Matrix4()
	{
		// 단위 행렬 만들기.
		memset(elements, 0, sizeof(float) * 16);
		m00 = m11 = m22 = m33 = 1.0f;
	}

	Matrix4::Matrix4(const Matrix4& other)
	{
		memcpy(elements, other.elements, sizeof(float) * 16);
	}
}