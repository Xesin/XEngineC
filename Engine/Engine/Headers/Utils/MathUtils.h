#pragma once
#include <stdlib.h>

static class MathUtils {
public:
	inline static int RandomInRange(int min, int max) {
		return min + (rand() % static_cast<int>(max - min + 1));
	}

	inline static float RandomInRange(float min, float max) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}
};