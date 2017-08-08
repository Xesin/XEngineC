#pragma once

#include "Properties.h"
#include "Layer.h"
#include "ObjectGroup.h"
#include "ImageLayer.h"

namespace tmx {
	class Group {
		Properties* properties;
		Layer* layer;
		ObjectGroup* objectGroup;
		ImageLayer* imageLayer;
		Group* group;

		char* name;
		char* offsetx;
		char* offsety;
		char* x;
		char* y;
		char* opacity;
		char* visible;
	};
}