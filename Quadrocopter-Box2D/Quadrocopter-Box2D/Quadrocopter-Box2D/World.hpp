//
//  World.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <vector>
#include <functional>
#include <Box2D/Box2D.h>

template <typename QuadrocopterType, typename ObstacleType>
class World;
class Quadrocopter1D;
class Quadrocopter2D;
class Obstacle1D;
class Obstacle2D;
typedef World<Quadrocopter1D, Obstacle1D> World1D;
typedef World<Quadrocopter2D, Obstacle2D> World2D;

#include "Quadrocopter1D.hpp"
#include "Quadrocopter2D.hpp"
#include "Obstacle1D.hpp"
#include "Obstacle2D.hpp"
#include "WorldBase.hpp"

template <typename QuadrocopterType, typename ObstacleType>
class World : public WorldBase<QuadrocopterType, ObstacleType> {

public:

	World ();
	~World ();

	b2World* world;
	b2Body* worldBody;
	
	void create ();
	void step () override;
	QuadrocopterType& createQuadrocopter () override;
	ObstacleType& createObstacle () override;
	
private:

	constexpr const static float timeStep = 1.0f / 20.0f;
	const static int velocityIterations = 6; // default 8
	const static int positionIterations = 2; // default 3

};

float degToRad (float deg);
float radToDeg (float rad);


template <typename QuadrocopterType, typename ObstacleType>
World<QuadrocopterType, ObstacleType>::World () : world(nullptr) {}

template <typename QuadrocopterType, typename ObstacleType>
World<QuadrocopterType, ObstacleType>::~World () {
	if (world != nullptr) delete world;
}

template <typename QuadrocopterType, typename ObstacleType>
void World<QuadrocopterType, ObstacleType>::create () {
	world = new b2World (b2Vec2 (0.0f, 2.0f)); //gravity in the world

	b2BodyDef worldBodyDef;
	worldBodyDef.type = b2BodyType::b2_staticBody;
	worldBodyDef.position.Set(0.0f, 0.0f);
	worldBody = world->CreateBody(&worldBodyDef);
}

template <typename QuadrocopterType, typename ObstacleType>
void World<QuadrocopterType, ObstacleType>::step () {
	WorldBase<QuadrocopterType, ObstacleType>::step ();
	world->Step(timeStep, velocityIterations, positionIterations);
}

template <typename QuadrocopterType, typename ObstacleType>
QuadrocopterType& World<QuadrocopterType, ObstacleType>::createQuadrocopter () {
	QuadrocopterType& q = WorldBase<QuadrocopterType, ObstacleType>::createQuadrocopter ();
	q.createIn(*this);
	return q;
}

template <typename QuadrocopterType, typename ObstacleType>
ObstacleType& World<QuadrocopterType, ObstacleType>::createObstacle () {
	return WorldBase<QuadrocopterType, ObstacleType>::createObstacle ();
}

#endif /* World_hpp */
