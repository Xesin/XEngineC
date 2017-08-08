#pragma once
#include "Properties.h"
#include "Object.h"

namespace tmx {
	class ObjectGroup {
	public:
		Properties* properties;
		Object* object;

		char* name;
		char* color;
		char* x;
		char* y;
		char* width;
		char* height;
		char* opacity;
		char* visible;
		char* offsetx;
		char* offsety;
		char* draworder;
	};
}