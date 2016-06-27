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

class QuadrocopterModel2DIFace {
public:

	constexpr static const float sensorsMagnitude = 10;
	constexpr static const float sensorsLength = 20;
	constexpr static const int sensorsCount = 32;
	
	virtual void setId (int id);
	virtual int getId () const;
	virtual void setCollided (bool collided);
	virtual bool isCollided () const;
	
	virtual void setTarget (const b2Vec2& pos) = 0;
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
	
	virtual std::vector<float>& getSensors () const = 0;
	
protected:

	int id = 0;
	bool collided = false;

};

#endif /* QuadrocopterModelIFace_hpp */
