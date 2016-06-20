//
//  World.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "World.hpp"

//World::World () :
//	world(b2Vec2 (0.0f, -10.0f)) //gravity in the world
//{
//
//	b2BodyDef worldBodyDef;
//	worldBodyDef.type = b2BodyType::b2_staticBody;
//	worldBodyDef.position.Set(0.0f, 0.0f);
//	worldBody = world.CreateBody(&worldBodyDef);
//}
//World::~World () {
//	
//}

World::World () : world(nullptr) {}

World::~World () {
	if (world != nullptr) delete world;
}

void World::create () {
	world = new b2World (b2Vec2 (0.0f, 2.0f)); //gravity in the world

	b2BodyDef worldBodyDef;
	worldBodyDef.type = b2BodyType::b2_staticBody;
	worldBodyDef.position.Set(0.0f, 0.0f);
	worldBody = world->CreateBody(&worldBodyDef);
}

void World::step () {
	world->Step(timeStep, velocityIterations, positionIterations);
}

float degToRad (float deg) {
	return M_PI * deg / 180;
}

float radToDeg (float rad) {
	return 180 * rad / M_PI;
}
