#pragma once
#include "Properties.h"
#include "Data.h"

namespace TiledImporter {
	class Layer{
	public:
		Properties* properties;
		Data* data;

		char* name;
		char* x;
		char* y;
		char* width;
		char* height;
		char* opacity;
		char* visible;
		char* offsetx;
		char* offsety;
	};
}