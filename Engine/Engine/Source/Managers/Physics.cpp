#include "Managers\Physics.h"
#include "Renderer\Renderer.h"
#include "Box2D\Box2D.h"


Physics::Physics(Renderer& ref)
{
	gravity = b2Vec2(0.0f, -25.0f);
	world = *(new b2World(gravity));
	world.SetDebugDraw(&ref);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	ref.SetFlags(flags);
	drawDebug = false;
}

void Physics::Update(float32 deltaTime)
{
	world.Step(deltaTime, 6, 2);
}
