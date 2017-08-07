#pragma once
#include "Data.h"

namespace TiledImporter {
	class Image {
	public:
		Data* data;
		char* format;
		char* id;
		char* source;
		char* trans;
		char* width;
		char* height;
	};
}