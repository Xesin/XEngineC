#include "Managers\Physics.h"


Physics::Physics()
{
	gravity = b2Vec2(0.0f, -25.0f);
	world = *(new b2World(gravity));
}

void Physics::Update(float32 deltaTime)
{
	world.Step(deltaTime, 6, 2);
}
