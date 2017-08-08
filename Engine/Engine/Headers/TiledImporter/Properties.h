#pragma once
#include "Property.h"
#include <map>

namespace tmx {
	class Properties {
		std::map<char*, Property*> properties;
	};
}