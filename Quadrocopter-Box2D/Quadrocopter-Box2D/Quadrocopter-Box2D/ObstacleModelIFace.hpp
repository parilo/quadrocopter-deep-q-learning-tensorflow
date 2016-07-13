//
//  ObstacleModelIFace.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ObstacleModelIFace_hpp
#define ObstacleModelIFace_hpp

class ObstacleModel2DIFace {
public:
	
	virtual void getCoords (
		float& centerX,
		float& centerY,
		float& a,
		float& b,
		float& angle
	) const = 0;

	virtual void getPoints (
		float& x0,
		float& y0,
		float& x1,
		float& y1,
		float& x2,
		float& y2,
		float& x3,
		float& y3
	) const = 0;
};

#endif /* ObstacleModelIFace_hpp */
