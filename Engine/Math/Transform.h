#pragma once
#include "Matrix33.h"

namespace nc{
	struct Transform {
		Vector2 position;
		float rotation;
		Vector2 scale = 1;

		Vector2 localPosition;
		float localRotation;
		Vector2 localScale = 1;


		Matrix33 matrix;


		Transform() {}
		Transform(const Vector2& position, float rotation = 0, float scale = 1) : position{ position }, rotation{ rotation }, scale{ scale }{}

		void Update();
		void Update(const Matrix33& mx);
	};

}