//
//  QuadrocopterDiscrete2D.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/06/16.
//
//

#include <Box2D/Box2D.h>

#include "QuadrocopterDiscrete2D.hpp"
#include "Lib.h"

QuadrocopterDiscrete2D::QuadrocopterDiscrete2D () {
	sensors.resize(sensorsCount);
}

void QuadrocopterDiscrete2D::createIn (WorldDiscrete2D& w) {
}

void QuadrocopterDiscrete2D::step () {
}



void QuadrocopterDiscrete2D::setCoords (const b2Vec2& pos, float angle) {
	posX = pos.x;
	posY = pos.y;
	this->angle = angle;
}

void QuadrocopterDiscrete2D::setVelocity (const b2Vec2& v) {
}

void QuadrocopterDiscrete2D::setAngularVelocity (float w) {
}

void QuadrocopterDiscrete2D::getPartsCoords (
	b2Vec2& bodyPos,
	b2Vec2& motor1Pos,
	b2Vec2& motor2Pos,
	float& bodyRotation,
	float& motor1Rotation,
	float& motor2Rotation
) const {
	bodyPos.x = posX;
	bodyPos.y = posY;
	
	static const float halfBodyLength = 0.2f;
	
	b2Vec2 posAdd (- halfBodyLength * sinf (angle), halfBodyLength * cosf(angle));
	motor1Pos = bodyPos + posAdd;
	motor2Pos = bodyPos - posAdd;
	
	bodyRotation = (angle + M_PI_2) * 180 / M_PI;
	motor1Rotation = bodyRotation;
	motor2Rotation = motor1Rotation;
}

void QuadrocopterDiscrete2D::getMotorPower (float& p1, float& p2) const {
	p1 = motor1power;
	p2 = motor2power;
}

void QuadrocopterDiscrete2D::setTarget (const b2Vec2& pos) {
	targetX = pos.x;
	targetY = pos.y;
}

void QuadrocopterDiscrete2D::getMainCoords (
	float& posX,
	float& posY,
	float& angle
) const {
	posX = this->posX;
	posY = this->posY;
	angle = this->angle;
}
