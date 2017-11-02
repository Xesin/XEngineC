#pragma once
#include "TiledImporter\Properties.h"
#include "TiledImporter\Tileset.h"
#include "TiledImporter\Layer.h"
#include "TiledImporter\ObjectGroup.h"
#include "TiledImporter\ImageLayer.h"
#include "TiledImporter\Group.h"
#include "pugixml.hpp"
#include <vector>

using namespace std;

namespace tmx {
	class Map {
	public:
		Map(){}

		Properties properties;
		vector<Tileset> tileSets;
		vector<Layer> layers;
		vector<ObjectGroup> objectGroups;
		vector<ImageLayer> imageLayers;
		vector<Group> groups;

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