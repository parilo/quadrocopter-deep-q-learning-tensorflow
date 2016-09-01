//
//  World.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "World.hpp"
#include "WorldObjectInfo.hpp"

float degToRad (float deg) {
	return M_PI * deg / 180;
}

float radToDeg (float rad) {
	return 180 * rad / M_PI;
}
