#pragma once
#include "Properties.h"
#include "Object.h"
#include <vector>

namespace tmx {
	class ObjectGroup {
	public:
		Properties properties;
		vector<Object> objects;

		const char* name;
		const char* color;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		const char* opacity;
		const char* visible;
		const char* offsetx;
		const char* offsety;
		const char* draworder;
	};
}