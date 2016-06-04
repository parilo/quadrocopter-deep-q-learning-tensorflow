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

template <typename QuadrocopterType>
class SimulationTmpl {
public:

	SimulationTmpl (int quadrocoptersCount);

	void step ();
	
	QuadrocopterType& getQuadrocopter (int index);
	float getQuadrocopterPosition (int index);
	
private:

	World world;
	std::vector<QuadrocopterType> quadrocopters;
	
};

template <typename QuadrocopterType>
SimulationTmpl<QuadrocopterType>::SimulationTmpl (int quadrocoptersCount) {
	world.create();
	for (int i=0; i<quadrocoptersCount; i++) {
		QuadrocopterType q;
		q.createIn(world);
		quadrocopters.push_back(q);
	}
}

template <typename QuadrocopterType>
void SimulationTmpl<QuadrocopterType>::step () {
	for (auto q : quadrocopters) {
		q.step();
	}
	world.step();
}

template <typename QuadrocopterType>
QuadrocopterType& SimulationTmpl<QuadrocopterType>::getQuadrocopter (int index) {
	return quadrocopters [index];
}

template <typename QuadrocopterType>
float SimulationTmpl<QuadrocopterType>::getQuadrocopterPosition (int index) {
	return quadrocopters [index].getPosition();
}

typedef SimulationTmpl<Quadrocopter1D> Simulation;
typedef SimulationTmpl<Quadrocopter2D> Simulation2D;

#endif /* Simulation_hpp */
