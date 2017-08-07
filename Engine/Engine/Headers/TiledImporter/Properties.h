#pragma once
#include "Property.h"
#include <map>

namespace TiledImporter {
	class Properties {
		std::map<char*, Property*> properties;
	};
}