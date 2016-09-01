//
//  Quadrocopter1D.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Quadrocopter1D_hpp
#define Quadrocopter1D_hpp

#include "World.hpp"

class Quadrocopter1D {

public:

	Quadrocopter1D ();
	virtual ~Quadrocopter1D ();
	
	void createIn (World1D& w);
	void setId (int id) {}
	void setCollided (bool collided) {}
	bool isCollided () const { return false; }

	float getPosition ();
	void setPosition (float pos);
	void setVelocity (float v);
	float getVelocity ();
	void setMotorPower (float p);
	
	void step ();
	
	void sense (const Obstacle1D& o) {}
	void clearSensors () {}

private:

	float motorPower = 0;

	b2Body* body;
};

#endif /* Quadrocopter1D_hpp */
