//
//  main.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 23/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include <iostream>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "Quadrocopter1D.hpp"
#include "Simulation.hpp"

int main(int argc, const char * argv[]) {

	Simulation s (1);
	s.getQuadrocopter(0).setVelocity(-10);
	s.getQuadrocopter(0).setMotorPower(10);

	for (int32 i = 0; i < 600; ++i) {

		s.step();
		
		float pos = s.getQuadrocopterPosition(0);

		printf("%4.2f\n", pos);
	}

	
	std::cout << "Hello, World!\n";
    return 0;
}
