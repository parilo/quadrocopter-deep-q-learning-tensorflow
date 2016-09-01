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
class WorldContactListener : public b2ContactListener {
public:
	
	WorldContactListener (World<QuadrocopterType, ObstacleType>& world) : world(world) {}
	
	void getObjects (b2Contact* contact, QuadrocopterType*& q, ObstacleType*& o) {
	
		void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();
		
		if (!dataA) return;
		if (!dataB) return;
		
		WorldObjectInfo& infoA = *static_cast<WorldObjectInfo*>(dataA);
		WorldObjectInfo& infoB = *static_cast<WorldObjectInfo*>(dataB);
		
		if (infoA.type == WorldObjectInfo::Type::Quadrocopter) {
			q = &world.getQuadrocopter (infoA.id);
			o = &world.getObstacle (infoB.id);
		} else if (infoA.type == WorldObjectInfo::Type::Obstacle) {
			q = &world.getQuadrocopter (infoB.id);
			o = &world.getObstacle (infoA.id);
		} else {
			return;
		}
	}

	void BeginContact(b2Contact* contact) override {
		QuadrocopterType* q;
		ObstacleType* o;
		getObjects (contact, q, o);
		q->setCollided(true);
	}

	void EndContact(b2Contact* contact) override {
		QuadrocopterType* q;
		ObstacleType* o;
		getObjects (contact, q, o);
		q->setCollided(false);
	}
	
private:

	World<QuadrocopterType, ObstacleType>& world;

};

template <typename QuadrocopterType, typename ObstacleType>
class World : public WorldBase<QuadrocopterType, ObstacleType> {

public:

	World ();
	~World () override;

	std::shared_ptr<b2World> world;
	b2Body* worldBody;
	WorldContactListener<QuadrocopterType, ObstacleType> contactListener;
	
	void create ();
	void step () override;
	QuadrocopterType& createQuadrocopter () override;
	ObstacleType& createObstacle () override;
	
	bool isPointInsideObstacles (const b2Vec2& point) {
		std::vector<ObstacleType>& os = WorldBase<QuadrocopterType, ObstacleType>::getObstacles ();
		for (auto& o : os) {
			if (o.containsPoint (point)) {
				return true;
			}
		}
		return false;
	}
	
private:

	constexpr const static float timeStep = 1.0f / 20.0f;
	const static int velocityIterations = 6; // default 8
	const static int positionIterations = 2; // default 3

};

float degToRad (float deg);
float radToDeg (float rad);


template <typename QuadrocopterType, typename ObstacleType>
World<QuadrocopterType, ObstacleType>::World () : world(nullptr), contactListener(*this) {}

template <typename QuadrocopterType, typename ObstacleType>
World<QuadrocopterType, ObstacleType>::~World () {}

template <typename QuadrocopterType, typename ObstacleType>
void World<QuadrocopterType, ObstacleType>::create () {
	world = std::shared_ptr<b2World>(new b2World (b2Vec2 (0, 10))); //gravity in the world

	world->SetContactListener (&contactListener);

	b2BodyDef worldBodyDef;
	worldBodyDef.type = b2BodyType::b2_staticBody;
	worldBodyDef.position.Set(0.0f, 0.0f);
	worldBody = world->CreateBody(&worldBodyDef);
}

template <typename QuadrocopterType, typename ObstacleType>
void World<QuadrocopterType, ObstacleType>::step () {
	WorldBase<QuadrocopterType, ObstacleType>::step ();
	world->Step(timeStep, velocityIterations, positionIterations);
	WorldBase<QuadrocopterType, ObstacleType>::afterStep ();
}

template <typename QuadrocopterType, typename ObstacleType>
QuadrocopterType& World<QuadrocopterType, ObstacleType>::createQuadrocopter () {
	QuadrocopterType& q = WorldBase<QuadrocopterType, ObstacleType>::createQuadrocopter ();
	q.createIn(*this);
	return q;
}

template <typename QuadrocopterType, typename ObstacleType>
ObstacleType& World<QuadrocopterType, ObstacleType>::createObstacle () {
	ObstacleType& o = WorldBase<QuadrocopterType, ObstacleType>::createObstacle ();
	o.createIn (*this);
	return o;
}

#endif /* World_hpp */
