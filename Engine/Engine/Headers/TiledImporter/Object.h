#pragma once
#include "Properties.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "Polyline.h"

namespace tmx {
	class Object {
	public:
		Properties properties;
		Ellipse ellipse;
		Polygon polygon;
		Polyline polyline;

		const char* id;
		const char* name;
		const char* type;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		const char* rotation;
		const char* gid;
		const char* visible;
	};
}