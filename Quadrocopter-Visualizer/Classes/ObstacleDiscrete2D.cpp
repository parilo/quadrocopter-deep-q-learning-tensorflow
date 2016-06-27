//
//  ObstacleDiscrete2D.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/06/16.
//
//

#include "ObstacleDiscrete2D.hpp"
#include "Lib.h"
#include <Box2D/Box2D.h>

void ObstacleDiscrete2D::step () {
	x += vx;
	y += vy;
	angle += w;
}

void ObstacleDiscrete2D::reset () {
	a = Lib::randFloat(5, 40);
	b = Lib::randFloat(5, 40);
	a2 = a * 0.5;
	b2 = b * 0.5;
	x = Lib::randFloat(-100, 100);
	y = Lib::randFloat(-100, 100);
	angle = Lib::randFloat(0, 2 * M_PI);
	
	vx = 0;//Lib::randFloat(-0.2, 0.2);
	vy = 0;//Lib::randFloat(-0.2, 0.2);
	w = 0;//Lib::randFloat(-0.02, 0.02);
}

bool ObstacleDiscrete2D::isCollidedWith (const QuadrocopterDiscrete2D& q) {

	//quadrocopter point in center of rectangle
	b2Vec2 Q (q.posX - x, q.posY - y);
	//rotating point of quadrocopter
	float c = cosf (-angle + M_PI_2);
	float s = sinf (-angle + M_PI_2);
	Q = b2Vec2 (Q.x * c - Q.y * s, Q.x * s + Q.y * c);
	
	if (
		Q.x > -a2 && Q.x < a2 &&
		Q.y > -b2 && Q.y < b2
	) {
		return true;
	} else {
		return false;
	}
}

void ObstacleDiscrete2D::getPoints (
	float& x0,
	float& y0,
	float& x1,
	float& y1,
	float& x2,
	float& y2,
	float& x3,
	float& y3
) const {

	float px0, py0, px1, py1, px2, py2, px3, py3;

	px0 = - a2; py0 = + b2;
	px1 = + a2; py1 = + b2;
	px2 = + a2; py2 = - b2;
	px3 = - a2; py3 = - b2;
	
	float c = cosf (angle + M_PI_2);
	float s = sinf (angle + M_PI_2);
	
	x0 = px0 * c - py0 * s + x; y0 = px0 * s + py0 * c + y;
	x1 = px1 * c - py1 * s + x; y1 = px1 * s + py1 * c + y;
	x2 = px2 * c - py2 * s + x; y2 = px2 * s + py2 * c + y;
	x3 = px3 * c - py3 * s + x; y3 = px3 * s + py3 * c + y;
}

void ObstacleDiscrete2D::getCoords (
	float& centerX,
	float& centerY,
	float& a,
	float& b,
	float& angle
) const {
	centerX = x;
	centerY = y;
	a = this->a;
	b = this->b;
	angle = (this->angle + M_PI_2) * 180 / M_PI;
}
