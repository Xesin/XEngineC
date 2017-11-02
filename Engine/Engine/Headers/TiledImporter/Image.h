#pragma once
#include "Data.h"

namespace tmx {
	class Image {
	public:
		Data* data;
		const char* format;
		int id;
		const WCHAR* source;
		const char* trans;
		int width;
		int height;
	};
}