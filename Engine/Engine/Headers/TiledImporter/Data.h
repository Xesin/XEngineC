#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
namespace tmx {
	class Data {
	public:

		vector<vector<string>> value;

		const char* encoding;
		const char* compression;
	};
}