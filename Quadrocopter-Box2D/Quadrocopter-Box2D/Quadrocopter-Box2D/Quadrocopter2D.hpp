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
#include <iostream>

#include "World.hpp"
#include "QuadrocopterModelIFace.hpp"
#include "WorldObjectInfo.hpp"

class Quadrocopter2D : public QuadrocopterModel2DIFace {

public:

	Quadrocopter2D ();
	~Quadrocopter2D () override;
	
	void createIn (World2D& w);

	const b2Vec2& getPosition ();
	void setCoords (const b2Vec2& pos, float angle) override;
	float getRotation ();
	void setVelocity (const b2Vec2& v) override;
	const b2Vec2& getVelocity ();
	void setAngularVelocity (float w) override;
	float getAngularVelocity ();
	
	void setLinearDamping (float d);
	void setAngularDamping (float d);
	float getLinearDamping ();
	float getAngularDamping ();
	
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
	
	World2D& getWorld ();
	bool isPointInsideObstacles (const b2Vec2& point);

protected:

	void getMainCoords (
		float& posX,
		float& posY,
		float& angle
	) const override;

private:

	float motor1Power = 0.2;
	float motor2Power = 0.2;

	b2BodyDef motor1Def;
	b2BodyDef motor2Def;

	b2Body* body;
	b2Body* motor1;
	b2Body* motor2;
	
	World2D* world;
	std::shared_ptr<WorldObjectInfo> worldObjectInfo;
	
};

#endif /* Quadrocopter2D_hpp */
