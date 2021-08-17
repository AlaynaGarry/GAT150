#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace nc {
	class SaveFile
	{
	public:
		int static LoadHighScore(const std::string& filename);
		void static WriteHighScore(const std::string& filename, int newHighscore);
	};
}