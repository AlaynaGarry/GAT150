#pragma once
#include "Json.h"

namespace nc {
	class ISserializable
	{
	public:
		virtual ~ISserializable();

		virtual bool Write(const rapidjson::Value& value) const = 0;
		virtual bool Read(const rapidjson::Value& value) = 0;

	};
}
