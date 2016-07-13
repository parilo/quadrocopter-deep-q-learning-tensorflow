//
//  ObstacleDiscrete2D.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/06/16.
//
//

#ifndef ObstacleDiscrete2D_hpp
#define ObstacleDiscrete2D_hpp

#include "QuadrocopterDiscrete2D.hpp"
#include "ObstacleModelIFace.hpp"

class ObstacleDiscrete2D : public ObstacleModel2DIFace {
public:
	
	void step ();
	void reset ();
	bool isCollidedWith (const QuadrocopterDiscrete2D& q);

	void getPoints (
		float& x0,
		float& y0,
		float& x1,
		float& y1,
		float& x2,
		float& y2,
		float& x3,
		float& y3
	) const override;

	void getCoords (
		float& centerX,
		float& centerY,
		float& a,
		float& b,
		float& angle
	) const override;

private:

	float a;
	float b;
	float a2;
	float b2;
	float x;
	float y;
	float angle;
	
	float vx;
	float vy;
	float w;

};

#endif /* ObstacleDiscrete2D_hpp */
