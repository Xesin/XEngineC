#pragma once
#include "Properties.h"
#include "Image.h"
#include "ObjectGroup.h"
#include "Animation.h"

namespace TiledImporter {
	class Tile {
	public:
		Properties* properties;
		Image* image;
		ObjectGroup* objectGroup;
		Animation* animation;

		char* id;
		char* type;
		char* terrain;
		char* probability;
	};
}