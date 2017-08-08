#pragma once
#include "Properties.h"

namespace tmx {
	class Terrain {
	public:
		Properties* properties;
		char* name;
		char* tile;
	};
}