#pragma once
#include "TerrainTypes.h"
#include "Tile.h"

namespace tmx {
	class Tileoffset {
	public:
		float x;
		float y;
	};

	class Tileset {
	public:
		Tileoffset tileoffset;
		Properties properties;
		Image image;
		TerrainTypes terrainTypes;
		Tile tile;

		const char* firstgid;
		const char* source;
		const char* name;
		const char* tilewidth;
		const char* tileheight;
		const char* spacing;
		const char* margin;
		const char* tilecount;
		const char* columns;
	};	
}