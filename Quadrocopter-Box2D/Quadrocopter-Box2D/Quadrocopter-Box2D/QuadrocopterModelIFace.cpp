//
//  QuadrocopterModelIFace.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 18/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "QuadrocopterModelIFace.hpp"

void QuadrocopterModel2DIFace::setId (int id)
{
	this->id = id;
}

int QuadrocopterModel2DIFace::getId () const
{
	return id;
}

void QuadrocopterModel2DIFace::setCollided (bool collided) {
	this->collided =collided;
	for (auto& s : getSensors()) {
		s = sensorsMagnitude;
	}
}

bool QuadrocopterModel2DIFace::isCollided () const {
	return collided;
}

void QuadrocopterModel2DIFace::clearSensors () {
	for (auto& s : sensors) {
		s = sensorsMagnitude;
	}
}

std::vector<float>& QuadrocopterModel2DIFace::getSensors () const {
	return sensors;
}

bool getLineSegmentsIntersection (
	float p0_x, float p0_y, //first line points
	float p1_x, float p1_y,
	float p2_x, float p2_y, //sencond lise points
	float p3_x, float p3_y,
	float& i_x, float& i_y,
	float& t//intersection point in part of length of first line
) {
    float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom;
    s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;

    denom = s10_x * s32_y - s32_x * s10_y;
    if (denom == 0)
        return false; // Collinear
    bool denomPositive = denom > 0;

    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
    s_numer = s10_x * s02_y - s10_y * s02_x;
    if ((s_numer < 0) == denomPositive)
        return false; // No collision

    t_numer = s32_x * s02_y - s32_y * s02_x;
    if ((t_numer < 0) == denomPositive)
        return false; // No collision

    if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
        return false; // No collision
    // Collision detected
    t = t_numer / denom;
    i_x = p0_x + (t * s10_x);
    i_y = p0_y + (t * s10_y);

    return true;
}

void QuadrocopterModel2DIFace::sense (const ObstacleModel2DIFace& o) {
	float x0, y0, x1, y1, x2, y2, x3, y3;
	o.getPoints (x0, y0, x1, y1, x2, y2, x3, y3);

	float posX;
	float posY;
	float angle;
	getMainCoords(posX, posY, angle);

	for (int i=0; i<sensorsCount; i++) {
		float sx = sensorsLength * sinf (i * 2 * M_PI / sensorsCount - angle + M_PI_2) + posX;
		float sy = sensorsLength * cosf (i * 2 * M_PI / sensorsCount - angle + M_PI_2) + posY;

		float ix, iy, t;
		float minT = sensors [i] / sensorsMagnitude;
		if (getLineSegmentsIntersection(posX, posY, sx, sy, x0, y0, x1, y1, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		if (getLineSegmentsIntersection(posX, posY, sx, sy, x1, y1, x2, y2, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		if (getLineSegmentsIntersection(posX, posY, sx, sy, x2, y2, x3, y3, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		if (getLineSegmentsIntersection(posX, posY, sx, sy, x3, y3, x0, y0, ix, iy, t)) {
			if (t < minT) minT = t;
		}
		sensors [i] = sensorsMagnitude * minT;
		
	}
}

void QuadrocopterModel2DIFace::setTarget (const b2Vec2& pos) {
	target = pos;
}

b2Vec2 QuadrocopterModel2DIFace::getTarget () {
	return target;
}
