#pragma once
#include "Properties.h"
#include "Data.h"

namespace tmx {
	class Layer{
	public:
		Properties properties;
		Data data;

		const char* name;
		const char* x;
		const char* y;
		const char* width;
		const char* height;
		const char* opacity;
		const char* visible;
		const char* offsetx;
		const char* offsety;
	};
}