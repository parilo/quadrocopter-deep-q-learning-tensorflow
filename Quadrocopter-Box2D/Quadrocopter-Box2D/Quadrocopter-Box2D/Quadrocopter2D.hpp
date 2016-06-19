//
//  Quadrocopter2D.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/05/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Quadrocopter2D_hpp
#define Quadrocopter2D_hpp

#include <vector>

#include "World.hpp"
#include "QuadrocopterModelIFace.hpp"

class Quadrocopter2D : public QuadrocopterModel2DIFace {

public:

	Quadrocopter2D ();
	virtual ~Quadrocopter2D ();
	
	void createIn (World& w);

	const b2Vec2& getPosition ();
	void setCoords (const b2Vec2& pos, float angle) override;
	float getRotation ();
	void setVelocity (const b2Vec2& v) override;
	const b2Vec2& getVelocity ();
	void setAngularVelocity (float w) override;
	float getAngularVelocity ();
	
	void getPartsCoords (
		b2Vec2& bodyPos,
		b2Vec2& motor1Pos,
		b2Vec2& motor2Pos,
		float& bodyRotation,
		float& motor1Rotation,
		float& motor2Rotation
	) const override;
	
	void getState (std::vector<float>& state);
	
	void setMotorPower (float p1, float p2);
	void setMotor1Power (float p1);
	void setMotor2Power (float p2);
	void getMotorPower (float& p1, float& p2) const override;
	
	void step ();

private:

	float motor1Power = 0.2;
	float motor2Power = 0.2;

	b2BodyDef motor1Def;
	b2BodyDef motor2Def;

	b2Body* body;
	b2Body* motor1;
	b2Body* motor2;
};

#endif /* Quadrocopter2D_hpp */
