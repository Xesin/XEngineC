#pragma once
#include "Map.h"
#include "pugixml.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace pugi;
using namespace std;

namespace tmx {
	class TiledImporter {
	public:
		static Map* LoadMap(const char* sourceFile, Map *outputMap) {			
			outputMap->xmlFile.load_file(sourceFile);

			xml_node mapNode = outputMap->xmlFile.child("map");

			outputMap->version = mapNode.attribute("version").value();
			outputMap->tiledversion = mapNode.attribute("tiledversion").value();
			outputMap->orientation = mapNode.attribute("orientation").value();
			outputMap->renderorder = mapNode.attribute("version").value();
			outputMap->width = mapNode.attribute("width").value();
			outputMap->height = mapNode.attribute("height").value();
			outputMap->tilewidth = mapNode.attribute("tilewidth").value();
			outputMap->tileheight = mapNode.attribute("tileheight").value();
			outputMap->hexsidelength = mapNode.attribute("hexsidelength").value();
			outputMap->staggeraxis = mapNode.attribute("staggeraxis").value();
			outputMap->staggerindex = mapNode.attribute("staggerindex").value();
			outputMap->backgroundcolor = mapNode.attribute("backgroundcolor").value();
			outputMap->nextobjectid = mapNode.attribute("nextobjectid").value();			
			
			GetPropertiesFromNode(mapNode, &outputMap->properties);
			GetTilesetFromNode(mapNode, outputMap->tileSets);
			GetLayersFromNode(mapNode, outputMap->layers);

			return outputMap;
		}

	private:
		static void GetPropertiesFromNode(xml_node node, Properties *outputProperties) {
			
		}

		static void GetTilesetFromNode(xml_node node, Tileset outputTileset[]) {
			int counter = 0;
			for (pugi::xml_node tileset = node.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
			{
				if (counter >= 10) {
					return;
				}

				ZeroMemory(&outputTileset[counter], sizeof(Tileset));

				outputTileset[counter].firstgid = tileset.attribute("firstgid").value();
				outputTileset[counter].source = tileset.attribute("source").value();
				outputTileset[counter].name = tileset.attribute("name").value();
				outputTileset[counter].tilewidth = tileset.attribute("tilewidth").value();
				outputTileset[counter].tileheight = tileset.attribute("tileheight").value();
				outputTileset[counter].spacing = tileset.attribute("spacing").value();
				outputTileset[counter].margin = tileset.attribute("margin").value();
				outputTileset[counter].tilecount = tileset.attribute("tilecount").value();
				outputTileset[counter].columns = tileset.attribute("columns").value();

				GetImageFromNode(tileset, &outputTileset[counter].image);

				counter++;
			}
		}

		static void GetLayersFromNode(xml_node node, Layer outputLayers[]) {
			int counter = 0;
			for (pugi::xml_node layerNode = node.child("layer"); layerNode; layerNode = layerNode.next_sibling("layer"))
			{
				if (counter >= 20) {
					return;
				}

				ZeroMemory(&outputLayers[counter], sizeof(Layer));

				outputLayers[counter].name = layerNode.attribute("name").value();
				outputLayers[counter].x = layerNode.attribute("x").value();
				outputLayers[counter].y = layerNode.attribute("y").value();
				outputLayers[counter].width = layerNode.attribute("width").value();
				outputLayers[counter].height = layerNode.attribute("height").value();
				outputLayers[counter].opacity = layerNode.attribute("opacity").value();
				outputLayers[counter].visible = layerNode.attribute("visible").value();
				outputLayers[counter].offsetx = layerNode.attribute("offsetx").value();
				outputLayers[counter].offsety = layerNode.attribute("offsety").value();

				GetDataFromNode(layerNode, &outputLayers[counter].data);

				counter++;
			}
		}

		static void GetDataFromNode(xml_node node, Data* outputData) {
			xml_node dataNode = node.child("data");

			string dataValue = dataNode.first_child().value();

			dataValue.erase(0, 1);

			istringstream iss(dataValue);
			vector<string> rows;

			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter(rows));

			vector<string>::iterator it = rows.begin();

			for (it; it != rows.end(); ++it) {
				string row = *it;
				istringstream iss2(row);
				vector<string> columns;
	

				while (iss2.good())
				{
					string substr;
					getline(iss2, substr, ',');
					columns.push_back(substr);
				}


				outputData->value.push_back(columns);
			}

			outputData->encoding = dataNode.attribute("encoding").value();
			outputData->compression = dataNode.attribute("compression").value();
		}

		static void GetImageFromNode(xml_node node, Image* outputImage) {

			xml_node imageNode = node.child("image");

			outputImage->format = imageNode.attribute("format").value();
			outputImage->id = imageNode.attribute("id").value();
			const char* source = imageNode.attribute("source").value();
			const size_t cSize = strlen(source) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, source, cSize);

			outputImage->source = wc;
			outputImage->trans = imageNode.attribute("trans").value();
			outputImage->width = imageNode.attribute("width").value();
			outputImage->height = imageNode.attribute("height").value();
		}
	};
}
