#pragma once
#include "Tile.h"

namespace TiledImporter {
	class Data {
	public:
		Tile* tile;

		char* encoding;
		char* compression;
	};
}