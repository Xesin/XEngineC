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

		int firstgid;
		const char* source;
		const char* name;
		int tilewidth;
		int tileheight;
		const char* spacing;
		const char* margin;
		const char* tilecount;
		const char* columns;
	};	
}