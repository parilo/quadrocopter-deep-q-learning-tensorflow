//
//  ObstacleDiscrete1D.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/06/16.
//
//

#ifndef ObstacleDiscrete1D_hpp
#define ObstacleDiscrete1D_hpp

#include "QuadrocopterDiscrete1D.hpp"

class ObstacleDiscrete1D {
public:
	
	void step () {}
	void reset ();
	bool isCollidedWith (const QuadrocopterDiscrete& q);

};

#endif /* ObstacleDiscrete1D_hpp */
