#pragma once
#include "Map.h"
#include "pugixml.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace pugi;
using namespace std;

namespace tmx {
	class TiledImporter {
	public:
		static Map* LoadMap(const wchar_t* sourceFile, Map *outputMap) {
			outputMap->xmlFile.load_file(sourceFile);

			xml_node mapNode = outputMap->xmlFile.child("map");

			outputMap->version = mapNode.attribute("version").value();
			outputMap->tiledversion = mapNode.attribute("tiledversion").value();
			outputMap->orientation = mapNode.attribute("orientation").value();
			outputMap->renderorder = mapNode.attribute("version").value();
			outputMap->width = std::stoi(mapNode.attribute("width").value());
			outputMap->height = std::stoi(mapNode.attribute("height").value());
			outputMap->tilewidth = std::stoi(mapNode.attribute("tilewidth").value());
			outputMap->tileheight = std::stoi(mapNode.attribute("tileheight").value());
			outputMap->hexsidelength = mapNode.attribute("hexsidelength").value();
			outputMap->staggeraxis = mapNode.attribute("staggeraxis").value();
			outputMap->staggerindex = mapNode.attribute("staggerindex").value();
			outputMap->backgroundcolor = mapNode.attribute("backgroundcolor").value();
			outputMap->nextobjectid = mapNode.attribute("nextobjectid").value();			
			
			GetPropertiesFromNode(mapNode, &outputMap->properties);
			GetTilesetFromNode(mapNode, &outputMap->tileSets);
			GetLayersFromNode(mapNode, &outputMap->layers);
			GetObjectGroupsFromNode(mapNode, &outputMap->objectGroups);


			return outputMap;
		}

	private:
		static void GetPropertiesFromNode(xml_node node, Properties *outputProperties) {
			
		}

		static void GetTilesetFromNode(xml_node node, vector<Tileset>* outputTileset) {
			int counter = 0;
			for (pugi::xml_node tileset = node.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
			{

				outputTileset->push_back(Tileset());
				outputTileset->at(counter).firstgid = std::stoi(tileset.attribute("firstgid").value());
				outputTileset->at(counter).source = tileset.attribute("source").value();
				outputTileset->at(counter).name = tileset.attribute("name").value();
				outputTileset->at(counter).tilewidth = std::stoi(tileset.attribute("tilewidth").value());
				outputTileset->at(counter).tileheight = std::stoi(tileset.attribute("tileheight").value());
				outputTileset->at(counter).spacing = tileset.attribute("spacing").value();
				outputTileset->at(counter).margin = tileset.attribute("margin").value();
				outputTileset->at(counter).tilecount = tileset.attribute("tilecount").value();
				outputTileset->at(counter).columns = tileset.attribute("columns").value();

				GetImageFromNode(tileset, &outputTileset->at(counter).image);

				counter++;
			}
		}

		static void GetLayersFromNode(xml_node node, vector<Layer>* outputLayers) {
			int counter = 0;
			for (pugi::xml_node layerNode = node.child("layer"); layerNode; layerNode = layerNode.next_sibling("layer"))
			{
				outputLayers->push_back(Layer());

				outputLayers->at(counter).name = layerNode.attribute("name").value();
				try {
					outputLayers->at(counter).x = std::stoi(layerNode.attribute("x").value());
				}
				catch (...) {
					outputLayers->at(counter).x = 0;
				}
				try {
					outputLayers->at(counter).y = std::stoi(layerNode.attribute("y").value());
				}
				catch (...) {
					outputLayers->at(counter).y = 0;
				}
				try {
					outputLayers->at(counter).width = std::stoi(layerNode.attribute("width").value());
				}
				catch (...) {
					outputLayers->at(counter).width = 0;
				}

				try {
					outputLayers->at(counter).height = std::stoi(layerNode.attribute("height").value());
				}
				catch (...) {
					outputLayers->at(counter).height = 0;
				}

				outputLayers->at(counter).opacity = layerNode.attribute("opacity").value();
				outputLayers->at(counter).visible = layerNode.attribute("visible").value();
				outputLayers->at(counter).offsetx = layerNode.attribute("offsetx").value();
				outputLayers->at(counter).offsety = layerNode.attribute("offsety").value();

				GetDataFromNode(layerNode, &outputLayers->at(counter).data);

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
			//const char* id = imageNode.attribute("id").value();
			//outputImage->id = std::stoi(imageNode.attribute("id").value());
			const char* source = imageNode.attribute("source").value();

			size_t size = strlen(source) + 1;
			wchar_t* sourceFile = new wchar_t[size];
			size_t outSize;
			mbstowcs_s(&outSize, sourceFile, size, source, size - 1);
			std::wcout << sourceFile << std::endl;

			outputImage->source = sourceFile;
			outputImage->trans = imageNode.attribute("trans").value();
			outputImage->width = std::stoi(imageNode.attribute("width").value());
			outputImage->height = std::stoi(imageNode.attribute("height").value());
		}

		static void GetObjectGroupsFromNode(xml_node node, vector<ObjectGroup>* outputGroups) {
			int counter = 0;
			for (pugi::xml_node groupNode = node.child("objectgroup"); groupNode; groupNode = groupNode.next_sibling("objectgroup"))
			{
				outputGroups->push_back(ObjectGroup());

				outputGroups->at(counter).name = groupNode.attribute("name").value();
				try {
					outputGroups->at(counter).x = std::stoi(groupNode.attribute("x").value());
				}
				catch (...) {
					outputGroups->at(counter).x = 0;
				}
				try {
					outputGroups->at(counter).y = std::stoi(groupNode.attribute("y").value());
				}
				catch (...) {
					outputGroups->at(counter).y = 0;
				}
				try {
					outputGroups->at(counter).width = std::stoi(groupNode.attribute("width").value());
				}
				catch (...) {
					outputGroups->at(counter).width = 0;
				}

				try {
					outputGroups->at(counter).height = std::stoi(groupNode.attribute("height").value());
				}
				catch (...) {
					outputGroups->at(counter).height = 0;
				}

				outputGroups->at(counter).opacity = groupNode.attribute("opacity").value();
				outputGroups->at(counter).visible = groupNode.attribute("visible").value();
				outputGroups->at(counter).offsetx = groupNode.attribute("offsetx").value();
				outputGroups->at(counter).offsety = groupNode.attribute("offsety").value();

				GetObjectsFromNode(groupNode, &outputGroups->at(counter).objects);

				counter++;
			}
		}

		static void GetObjectsFromNode(xml_node node, vector<Object> *outputObjects) {
			int counter = 0;
			for (pugi::xml_node groupNode = node.child("object"); groupNode; groupNode = groupNode.next_sibling("object"))
			{
				outputObjects->push_back(Object());
				try {
					outputObjects->at(counter).x = std::stoi(groupNode.attribute("x").value());
				}
				catch (...) {
					outputObjects->at(counter).x = 0;
				}
				try {
					outputObjects->at(counter).y = std::stoi(groupNode.attribute("y").value());
				}
				catch (...) {
					outputObjects->at(counter).y = 0;
				}
				try {
					outputObjects->at(counter).width = std::stoi(groupNode.attribute("width").value());
				}
				catch (...) {
					outputObjects->at(counter).width = 0;
				}

				try {
					outputObjects->at(counter).height = std::stoi(groupNode.attribute("height").value());
				}
				catch (...) {
					outputObjects->at(counter).height = 0;
				}

				outputObjects->at(counter).visible = groupNode.attribute("visible").value();

				counter++;
			}
		}


	};
}
