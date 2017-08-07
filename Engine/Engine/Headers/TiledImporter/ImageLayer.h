#pragma once
#include "ImageLayer.h"
#include "Image.h"

namespace TiledImporter {
	class ImageLayer {
	public:
		Properties* properties;
		Image* image;

		char* name;
		char* offsetx;
		char* offsety;
		char* x;
		char* y;
		char* opacity;
		char* visible;
	};
}