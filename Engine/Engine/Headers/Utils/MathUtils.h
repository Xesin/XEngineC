#pragma once
#include <stdlib.h>

class MathUtils {
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

struct Vector2 {
public:
	Vector2() {
		x = 0.f;
		y = 0.f;
	}

	Vector2(float currX, float currY) {
		x = currX;
		y = currY;
	}

	void operator +=(Vector2 other) {
		x += other.x;
		y += other.y;
	}

public:
	float x = 0.f;
	float y = 0.f;
};

struct Rotation {
public:
	void operator +=(Rotation other) {
		angles += other.angles;
	}

public:
	float angles = 0.f;
};