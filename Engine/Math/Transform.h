#pragma once
#include "Matrix33.h"
#include "Core/Serializable.h"

namespace nc{
	struct Transform : public ISserializable {
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

		// Inherited via ISserializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	};

}