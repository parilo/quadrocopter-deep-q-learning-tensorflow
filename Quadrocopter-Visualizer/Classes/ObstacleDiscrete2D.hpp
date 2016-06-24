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
	float x;
	float y;
	float angle;
	
	float vx;
	float vy;
	float w;

};

#endif /* ObstacleDiscrete2D_hpp */
