//
//  Quadrocopter2D.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/05/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include <iostream>

#include "Quadrocopter2D.hpp"

Quadrocopter2D::Quadrocopter2D () {
	sensors.resize(sensorsCount);
}

Quadrocopter2D::~Quadrocopter2D () {};

void Quadrocopter2D::createIn (World2D& w) {

	world = &w;
	worldObjectInfo = std::shared_ptr<WorldObjectInfo> (new WorldObjectInfo (WorldObjectInfo::Type::Quadrocopter, id));

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	bodyDef.linearDamping = 0.0f;
	bodyDef.angularDamping = 0.1f;
//	bodyDef.fixedRotation = true;

	body = w.world->CreateBody(&bodyDef);
	body->SetUserData(worldObjectInfo.get());
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.4, 0.1);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 4.0f; // 0.4 x 0.4 x 4 = 0.64 kg
	fixtureDef.friction = 0.3f;
	//filtering collisions between quadrocopters
	fixtureDef.filter.categoryBits = 0x0002;
	fixtureDef.filter.maskBits = 0x0004;
	body->CreateFixture(&fixtureDef);



	b2BodyDef motorDef;
	motorDef.type = b2_dynamicBody;
	motorDef.position.Set(-0.25f, 0.0f);
	motorDef.linearDamping = 0.0f;
	motorDef.angularDamping = 0.1f;
	motor1Def = motorDef;

	motor1 = w.world->CreateBody(&motorDef);
	motor1->SetUserData(worldObjectInfo.get());
	b2PolygonShape motorShape;
	motorShape.SetAsBox(0.1f, 0.2f);
	
	b2FixtureDef motorFixtureDef;
	motorFixtureDef.shape = &motorShape;
	motorFixtureDef.density = 4.0f; // 0.4 x 0.4 x 4 = 0.64 kg
	motorFixtureDef.friction = 0.3f;
	//filtering collisions between quadrocopters
	motorFixtureDef.filter.categoryBits = 0x0002;
	motorFixtureDef.filter.maskBits = 0x0004;
	
	motor1->CreateFixture(&motorFixtureDef);

	motorDef.position.Set(0.25f, 0.0f);
	motor2Def = motorDef;
	motor2 = w.world->CreateBody(&motorDef);
	motor2->SetUserData(worldObjectInfo.get());
	motor2->CreateFixture(&motorFixtureDef);

	b2WeldJointDef motor1Joint;
	motor1Joint.Initialize(body, motor1, motor1Def.position);
	motor1Joint.collideConnected = false;
	w.world->CreateJoint(&motor1Joint);

	b2WeldJointDef motor2Joint;
	motor2Joint.Initialize(body, motor2, motor2Def.position);
	motor2Joint.collideConnected = false;
	w.world->CreateJoint(&motor2Joint);
}

const b2Vec2& Quadrocopter2D::getPosition () {
	return body->GetPosition ();
}

void Quadrocopter2D::setCoords (const b2Vec2& pos, float angle) {

	angle = degToRad(angle);

//	body->SetLinearVelocity(b2Vec2 (0, 0));
//	motor1->SetLinearVelocity(b2Vec2 (0, 0));
//	motor2->SetLinearVelocity(b2Vec2 (0, 0));

	body->SetTransform (pos, angle);
	motor1->SetTransform(body->GetWorldPoint(motor1Def.position), angle);
	motor2->SetTransform(body->GetWorldPoint(motor2Def.position), angle);
}

float Quadrocopter2D::getRotation () {
	return body->GetAngle ();
}

void Quadrocopter2D::setVelocity (const b2Vec2& v) {
	body->SetLinearVelocity (v);
	motor1->SetLinearVelocity(v);
	motor2->SetLinearVelocity(v);
}

const b2Vec2& Quadrocopter2D::getVelocity () {
	return body->GetLinearVelocity ();
}

void Quadrocopter2D::setAngularVelocity (float w) {
	body->SetAngularVelocity (w);
}

float Quadrocopter2D::getAngularVelocity () {
	return body->GetAngularVelocity ();
}

void Quadrocopter2D::setMotorPower (float p1, float p2) {
	motor1Power = p1;
	motor2Power = p2;
}

void Quadrocopter2D::setMotor1Power (float p1) {
	motor1Power = p1;
}

void Quadrocopter2D::setMotor2Power (float p2) {
	motor2Power = p2;
}

void Quadrocopter2D::step () {
	motor1->ApplyForceToCenter (motor1->GetWorldVector(b2Vec2(0, -motor1Power)), true);
	motor2->ApplyForceToCenter (motor2->GetWorldVector(b2Vec2(0, -motor2Power)), true);
}

void Quadrocopter2D::getPartsCoords (
	b2Vec2& bodyPos,
	b2Vec2& motor1Pos,
	b2Vec2& motor2Pos,
	float& bodyRotation,
	float& motor1Rotation,
	float& motor2Rotation
) const {
	bodyPos = body->GetPosition ();
	motor1Pos = motor1->GetPosition ();
	motor2Pos = motor2->GetPosition ();
	bodyRotation = radToDeg(body->GetAngle ());
	motor1Rotation = radToDeg(motor1->GetAngle ());
	motor2Rotation = radToDeg(motor2->GetAngle ());
}

void Quadrocopter2D::getState (std::vector<float>& state) {
//	state.assign(state.size(), 0);

	state [0] = body->GetPosition().x;
	state [1] = body->GetPosition().y;
	state [2] = sinf(body->GetAngle());
	state [3] = cosf(body->GetAngle());
	state [4] = body->GetLinearVelocity().x;
	state [5] = body->GetLinearVelocity().y;
	state [6] = body->GetAngularVelocity();

	//next 10 floats are sparse coding of target vector
//	float targetAngle = atan2f(state[0], state[1]) * 180 / M_PI + 180;
//	int angleIndex = floorf (targetAngle / 36);
//	state [angleIndex + 7] = 1;

//	std::cout << state[2] << " " << state[3] << std::endl;
}

void Quadrocopter2D::getMotorPower (float& p1, float& p2) const {
	p1 = motor1Power;
	p2 = motor2Power;
}

void Quadrocopter2D::getMainCoords (
	float& posX,
	float& posY,
	float& angle
) const {
	posX = body->GetPosition ().x;
	posY = body->GetPosition ().y;
	angle = body->GetAngle () - M_PI_2;
}

void Quadrocopter2D::setLinearDamping (float d) {
	body->SetLinearDamping(d);
}

void Quadrocopter2D::setAngularDamping (float d) {
	body->SetAngularDamping(d);
}

float Quadrocopter2D::getLinearDamping () {
	return body->GetLinearDamping();
}

float Quadrocopter2D::getAngularDamping () {
	return body->GetAngularDamping();
}

World2D& Quadrocopter2D::getWorld () {
	return *world;
}

bool Quadrocopter2D::isPointInsideObstacles (const b2Vec2& point) {
	return world->isPointInsideObstacles(point);
}
