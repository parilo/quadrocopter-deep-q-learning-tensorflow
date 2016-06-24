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
	a = Lib::randFloat(10, 40);
	b = Lib::randFloat(10, 40);
	x = Lib::randFloat(-100, 100);
	y = Lib::randFloat(-100, 100);
	angle = Lib::randFloat(0, 2 * M_PI);
	
	vx = Lib::randFloat(-0.05, 0.05);
	vy = Lib::randFloat(-0.05, 0.05);
	w = Lib::randFloat(-0.02, 0.02);
}

bool ObstacleDiscrete2D::isCollidedWith (const QuadrocopterDiscrete2D& q) {

	b2Vec2 AA (-a/2, +b/2);
	b2Vec2 BB (-a/2, -b/2);
	b2Vec2 CC (+a/2, -b/2);
	float c = cosf (angle);
	float s = cosf (angle);
	b2Vec2 A (AA.x * c - AA.y * s, AA.x * s + AA.y * c);
	b2Vec2 B (BB.x * c - BB.y * s, BB.x * s + BB.y * c);
	b2Vec2 C (CC.x * c - CC.y * s, CC.x * s + CC.y * c);
	b2Vec2 M (q.posX - x, q.posY - y);
	b2Vec2 AB = B - A;
	b2Vec2 AM = M - A;
	b2Vec2 BC = C - B;
	b2Vec2 BM = M - B;

	float ABAM = b2Dot(AB, AM);
	float ABAB = b2Dot(AB, AB);
	float BCBM = b2Dot(BC, BM);
	float BCBC = b2Dot(BC, BC);

	if (
		0 <= ABAM && ABAM <= ABAB &&
		0 <= BCBM && BCBM <= BCBC
	) {
//CCLOG ("--- collide calc: %f %f %f %f q: %f %f A: %f %f B: %f %f C: %f %f M: %f %f AB: %f %f AM: %f %f BC: %f %f BM: %f %f ABAM: %f ABAB: %f BCBM: %f BCBC: %f",
//	x,
//	y,
//	a,
//	b,
//	q.posX,
//	q.posY,
//	A.x, A.y,
//	B.x, B.y,
//	C.x, C.y,
//	M.x, M.y,
//	AB.x, AB.y,
//	AM.x, AM.y,
//	BC.x, BC.y,
//	BM.x, BM.y,
//	ABAM,
//	ABAB,
//	BCBM,
//	BCBC
//);
		return true;
		
	} else {
		return false;
	}

//http://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not
//http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
//0 <= dot(AB,AM) <= dot(AB,AB) &&
//0 <= dot(BC,BM) <= dot(BC,BC)
//AB is vector AB, with coordinates (Bx-Ax,By-Ay), and dot(U,V) is the dot product of vectors U and V: Ux*Vx+Uy*Vy
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
