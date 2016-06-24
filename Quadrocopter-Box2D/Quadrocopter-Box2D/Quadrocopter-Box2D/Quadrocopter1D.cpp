//
//  Quadrocopter1D.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "Quadrocopter1D.hpp"
#include "World.hpp"

Quadrocopter1D::Quadrocopter1D () {}

Quadrocopter1D::~Quadrocopter1D () {};

void Quadrocopter1D::createIn (World1D& w) {

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	bodyDef.linearDamping = 0.0f;
	bodyDef.angularDamping = 0.1f;
//	bodyDef.fixedRotation = true;
	body = w.world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.4f, 0.4f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 4.0f; // 0.4 x 0.4 x 4 = 0.64 kg
	fixtureDef.friction = 0.3f;
	//filtering collisions between quadrocopters
	fixtureDef.filter.categoryBits = 0x0002;
	fixtureDef.filter.maskBits = 0x0004;
	body->CreateFixture(&fixtureDef);
	
	b2PrismaticJointDef lockY;
	lockY.localAxisA.Set(1, 0);
	lockY.bodyA = w.worldBody;
	lockY.bodyB = body;
	lockY.localAnchorA = w.worldBody->GetLocalPoint(bodyDef.position);
	lockY.localAnchorB = body->GetLocalPoint(bodyDef.position);
	w.world->CreateJoint(&lockY);
}

float Quadrocopter1D::getPosition () {
	return body->GetPosition().x;
}

void Quadrocopter1D::setPosition (float pos) {
	body->SetTransform(b2Vec2(pos, 0), 0);
}

void Quadrocopter1D::setVelocity (float v) {
	body->SetLinearVelocity(b2Vec2(v, 0));
}

float Quadrocopter1D::getVelocity () {
	return body->GetLinearVelocity().x;
}

void Quadrocopter1D::setMotorPower (float p) {
	motorPower = p;
}

void Quadrocopter1D::step () {
	body->ApplyForce(b2Vec2(motorPower, 0), body->GetWorldCenter(), true);
}
