//
//  World.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <Box2D/Box2D.h>

class World {

public:

	World ();
	~World ();

	b2World* world;
	b2Body* worldBody;
	
	void create ();
	void step ();

private:

	constexpr const static float timeStep = 1.0f / 20.0f;
	const static int velocityIterations = 6; // default 8
	const static int positionIterations = 2; // default 3

};

float degToRad (float deg);
float radToDeg (float rad);

#endif /* World_hpp */
