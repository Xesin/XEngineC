#pragma once
#include "TiledImporter\Properties.h"
#include "TiledImporter\Tileset.h"
#include "TiledImporter\Layer.h"
#include "TiledImporter\ObjectGroup.h"
#include "TiledImporter\ImageLayer.h"
#include "TiledImporter\Group.h"

namespace TiledImporter {
	class Map {
	public:
		Properties* properties;
		Tileset* tileSets;
		Layer* layers;
		ObjectGroup* objectGroups;
		ImageLayer* imageLayers;
		Group* groups;

		char* version;
		char* tiledversion;
		char* orientation;
		char* renderorder;
		char* width;
		char* height;
		char* tilewidth;
		char* tileheight;
		char* hexsidelength;
		char* staggeraxis;
		char* staggerindex;
		char* backgroundcolor;
		char* nextobjectid;
	};
}