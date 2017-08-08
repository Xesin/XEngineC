#pragma once
#include "Properties.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "Polyline.h"

namespace tmx {
	class Object {
		Properties* properties;
		Ellipse* ellipse;
		Polygon* polygon;
		Polyline polyline;

		char* id;
		char* name;
		char* type;
		char* x;
		char* y;
		char* width;
		char* height;
		char* rotation;
		char* gid;
		char* visible;
	};
}