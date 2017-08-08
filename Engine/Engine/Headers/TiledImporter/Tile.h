#pragma once
#include "Properties.h"
#include "TiledImporter\Image.h"
#include "ObjectGroup.h"
#include "Animation.h"

namespace tmx {
	class Tile {
	public:
		Properties* properties;
		tmx::Image* image;
		ObjectGroup* objectGroup;
		Animation* animation;

		char* id;
		char* type;
		char* terrain;
		char* probability;
	};
}