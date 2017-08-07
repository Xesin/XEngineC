#pragma once
#include "Image.h"
#include "TerrainTypes.h"
#include "Tile.h"

namespace TiledImporter {
	class Tileoffset {
	public:
		float x;
		float y;
	};

	class Tileset {
	public:
		Tileoffset* tileoffset;
		Properties* properties;
		Image* image;
		TerrainTypes* terrainTypes;
		Tile* tile;
		char* firstgid;
		char* source;
		char* name;
		char* tilewidth;
		char* tileheight;
		char* spacing;
		char* margin;
		char* tilecount;
		char* columns;
	};	
}