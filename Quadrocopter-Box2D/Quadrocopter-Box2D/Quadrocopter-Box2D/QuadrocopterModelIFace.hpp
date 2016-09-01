//
//  QuadrocopterModelIFace.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 18/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef QuadrocopterModelIFace_hpp
#define QuadrocopterModelIFace_hpp

#include <vector>
#include <Box2D/Box2D.h>

#include "ObstacleModelIFace.hpp"

class QuadrocopterModel2DIFace {
public:

	constexpr static const float sensorsMagnitude = 10;
	constexpr static const float sensorsLength = 80;
	constexpr static const int sensorsCount = 32;
	
	virtual ~QuadrocopterModel2DIFace () {}
	
	virtual void setId (int id);
	virtual int getId () const;
	virtual void setCollided (bool collided);
	virtual bool isCollided () const;
	
	virtual void setTarget (const b2Vec2& pos);
	b2Vec2 getTarget ();
	virtual void setCoords (const b2Vec2& pos, float angle) = 0;
	virtual void setVelocity (const b2Vec2& v) = 0;
	virtual void setAngularVelocity (float w) = 0;
	
	virtual void getPartsCoords (
		b2Vec2& bodyPos,
		b2Vec2& motor1Pos,
		b2Vec2& motor2Pos,
		float& bodyRotation,
		float& motor1Rotation,
		float& motor2Rotation
	) const = 0;

	virtual void getMotorPower (float& p1, float& p2) const = 0;
	
	void sense (const ObstacleModel2DIFace& o);
	virtual std::vector<float>& getSensors () const;
	void clearSensors ();
	
protected:

	int id = 0;
	bool collided = false;
	b2Vec2 target;

	mutable std::vector<float> sensors;

	/**
		coords of the center of quadrotor
	*/
	virtual void getMainCoords (
		float& posX,
		float& posY,
		float& angle
	) const = 0;

};

#endif /* QuadrocopterModelIFace_hpp */
