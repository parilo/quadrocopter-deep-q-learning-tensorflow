//
//  main.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 07/05/16.
//
//

#include <iostream>

#include "main.h"
#include "QuadrocopterSimulator.hpp"

QuadrocopterSimulatorContMLPSeq2D sim;

int main(int argc, const char * argv[]) {

	sim.init();
	sim.startActWorkers();
	sim.startTrainWorkers();
	sim.join();
	
	std::cout << "execution ended\n";
    return 0;
}
