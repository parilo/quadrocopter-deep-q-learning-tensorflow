//
//  Simulation.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 27/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Simulation_hpp
#define Simulation_hpp

#include <vector>

#include "World.hpp"
#include "Quadrocopter1D.hpp"
#include "Quadrocopter2D.hpp"

template <typename WorldType, typename QuadrocopterType>
class SimulationTmpl {
public:

	SimulationTmpl (int quadrocoptersCount);
	SimulationTmpl (int quadrocoptersCount, int obstaclesCount);

	void step ();
	
	WorldType& getWorld () {
		return world;
	}
	
	QuadrocopterType& getQuadrocopter (int index);
	float getQuadrocopterPosition (int index);
	
private:

	WorldType world;
	
};

template <typename WorldType, typename QuadrocopterType>
SimulationTmpl<WorldType, QuadrocopterType>::SimulationTmpl (int quadrocoptersCount) {
	world.create();
	for (int i=0; i<quadrocoptersCount; i++) {
		world.createQuadrocopter ();
	}
}

template <typename WorldType, typename QuadrocopterType>
SimulationTmpl<WorldType, QuadrocopterType>::SimulationTmpl (int quadrocoptersCount, int obstaclesCount) : SimulationTmpl (quadrocoptersCount)
{
	for (int i=0; i<obstaclesCount; i++) {
		world.createObstacle ();
	}
}

template <typename WorldType, typename QuadrocopterType>
void SimulationTmpl<WorldType, QuadrocopterType>::step () {
	world.step();
}

template <typename WorldType, typename QuadrocopterType>
QuadrocopterType& SimulationTmpl<WorldType, QuadrocopterType>::getQuadrocopter (int index) {
	return world.getQuadrocopters () [index];
}

template <typename WorldType, typename QuadrocopterType>
float SimulationTmpl<WorldType, QuadrocopterType>::getQuadrocopterPosition (int index) {
	return world.getQuadrocopters () [index].getPosition();
}

typedef SimulationTmpl<World1D, Quadrocopter1D> Simulation;
typedef SimulationTmpl<World2D, Quadrocopter2D> Simulation2D;

#endif /* Simulation_hpp */
