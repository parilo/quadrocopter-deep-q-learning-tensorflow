//
//  Obstacle2D.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "Obstacle2D.hpp"

void Obstacle2D::getCoords (
	float& centerX,
	float& centerY,
	float& a,
	float& b,
	float& angle
) const {}

void Obstacle2D::getPoints (
	float& x0,
	float& y0,
	float& x1,
	float& y1,
	float& x2,
	float& y2,
	float& x3,
	float& y3
) const {
	x0 = 10;
	y0 = 10;
	x1 = 20;
	y1 = 10;
	x2 = 20;
	y2 = 20;
	x3 = 10;
	y3 = 20;
}
