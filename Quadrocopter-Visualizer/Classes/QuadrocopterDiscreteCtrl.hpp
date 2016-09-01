//
//  QuadrocopterDiscreteCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 22/05/16.
//
//

#ifndef QuadrocopterDiscreteCtrl_hpp
#define QuadrocopterDiscreteCtrl_hpp

#include "Simulation.hpp"
#include "QuadrocopterDiscrete1D.hpp"

class QuadrocopterDiscreteCtrl {
public:
	
	QuadrocopterDiscreteCtrl (int id, QuadrocopterDiscrete& simulationModel);
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	double getReward ();
	int getPosition ();
	void setState (const std::vector<float> state) {}
	QuadrocopterDiscrete& getModel ();
	void onTrainStep (int trainStepIndex) {}
	void onSimulationStep (int stepIndex) {}

private:

	int id;
	long action = -1;
	int timeReward = 0;
	double reward;
	int position = 0;
	int prevPosition = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;
	
	QuadrocopterDiscrete& simulationModel;

};

#endif /* QuadrocopterDiscreteCtrl_hpp */
