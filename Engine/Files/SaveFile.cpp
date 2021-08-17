#include "SaveFile.h"

namespace nc {
	int SaveFile::LoadHighScore(const std::string& filename) {
		int highscore = -1;
		std::fstream my_file;
		my_file.open(filename, std::ios::in);

		if (my_file.is_open()) {
			my_file >> highscore;
			my_file.close();
		}
		return highscore;
	}
	void SaveFile::WriteHighScore(const std::string& filename, int newHighscore) {
		std::fstream my_file;
		my_file.open(filename, std::ios::out);
		if (my_file.is_open()) {
			my_file << newHighscore;
			my_file.close();
		}
	}

}