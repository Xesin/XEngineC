#pragma once
#include "Properties.h"
#include "Data.h"

namespace tmx {
	class Layer{
	public:
		Properties properties;
		Data data;

		const char* name;
		int x;
		int y;
		int width;
		int height;
		const char* opacity;
		const char* visible;
		const char* offsetx;
		const char* offsety;
	};
}