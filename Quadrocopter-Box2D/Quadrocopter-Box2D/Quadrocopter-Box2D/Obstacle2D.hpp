//
//  Obstacle2D.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Obstacle2D_hpp
#define Obstacle2D_hpp

#include "ObstacleModelIFace.hpp"

class Obstacle2D : public ObstacleModel2DIFace {
public:

	void step () {}
	void reset () {}
	
	void getCoords (
		float& centerX,
		float& centerY,
		float& a,
		float& b,
		float& angle
	) const override;

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
	
};

#endif /* Obstacle2D_hpp */
