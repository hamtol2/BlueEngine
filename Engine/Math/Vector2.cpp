#include "Vector2.h"
#include <Windows.h>
#include <cmath>

namespace Blue
{
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
	const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);

	std::wstring Vector2::ToString()
	{
		wchar_t buffer[256];
		swprintf_s(buffer, 256, TEXT("(%f,%f)"), x, y);

		return buffer;
	}

	float Vector2::Length()
	{
		return sqrtf(x * x + y * y);
	}

	float Vector2::Dot(const Vector2& other)
	{
		return (x * other.x) + (y * other.y);
	}

	Vector2 Vector2::Normalized()
	{
		float length = Length();
		return Vector2(x / length, y / length);
	}

	bool Vector2::Equals(const Vector2& other)
	{
		return x == other.x && y == other.y;
	}
}