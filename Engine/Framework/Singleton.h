#pragma once
#include <iostream>

namespace nc {
	template<class T>

	class Singleton {
	public: 
		Singleton(const Singleton&) = delete;
		Singleton& operator = (const Singleton&) = delete;

		static T& Instance() {
			static T instance;
			return instance;
		}

		//void print() { std::cout << "Hello\n" << std::endl; }
	protected:
		Singleton() {}
	};
}