#pragma once
#include "TiledImporter\Properties.h"
#include "TiledImporter\Tileset.h"
#include "TiledImporter\Layer.h"
#include "TiledImporter\ObjectGroup.h"
#include "TiledImporter\ImageLayer.h"
#include "TiledImporter\Group.h"
#include "pugixml.hpp"

namespace tmx {
	class Map {
	public:
		Properties properties;
		Tileset tileSets[10];
		Layer layers[20];
		ObjectGroup objectGroups[10];
		ImageLayer imageLayers[5];
		Group groups[10];

		pugi::xml_document xmlFile;

		const char* version;
		const char* tiledversion;
		const char* orientation;
		const char* renderorder;
		int width = 0;
		int height = 0;
		int tilewidth = 0;
		int tileheight = 0;
		const char* hexsidelength;
		const char* staggeraxis;
		const char* staggerindex;
		const char* backgroundcolor;
		const char* nextobjectid;
	};
}