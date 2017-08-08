#pragma once
#include "Data.h"

namespace tmx {
	class Image {
	public:
		Data* data;
		const char* format;
		const char* id;
		const WCHAR* source;
		const char* trans;
		const char* width;
		const char* height;
	};
}