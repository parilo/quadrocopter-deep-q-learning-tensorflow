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

void QuadrocopterDiscrete2D::sense (const ObstacleDiscrete2D& o) {
	float x0, y0, x1, y1, x2, y2, x3, y3;
	o.getPoints (x0, y0, x1, y1, x2, y2, x3, y3);

	for (int i=0; i<sensorsCount; i++) {
		float sx = sensorsLength * sinf (i * 2 * M_PI / sensorsCount - angle + M_PI_2) + posX;
		float sy = sensorsLength * cosf (i * 2 * M_PI / sensorsCount - angle + M_PI_2) + posY;

		float ix, iy, t;
		float minT = sensors [i] / sensorsMagnitude;
		if (Lib::getLineSegmentsIntersection(posX, posY, sx, sy, x0, y0, x1, y1, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		if (Lib::getLineSegmentsIntersection(posX, posY, sx, sy, x1, y1, x2, y2, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		if (Lib::getLineSegmentsIntersection(posX, posY, sx, sy, x2, y2, x3, y3, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		if (Lib::getLineSegmentsIntersection(posX, posY, sx, sy, x3, y3, x0, y0, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		sensors [i] = sensorsMagnitude * minT;
		
	}
}

void QuadrocopterDiscrete2D::clearSensors () {
	for (auto& s : sensors) {
		s = sensorsMagnitude;
	}
}

std::vector<float>& QuadrocopterDiscrete2D::getSensors () const {
	return sensors;
}
