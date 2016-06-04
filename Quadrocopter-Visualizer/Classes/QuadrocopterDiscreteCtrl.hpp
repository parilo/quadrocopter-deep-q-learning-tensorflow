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

class QuadrocopterDiscreteCtrl {
public:
	
	QuadrocopterDiscreteCtrl (int id);
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	double getReward ();
	int getPosition ();

private:

	int id;
	long action = -1;
	int timeReward = 0;
	double reward;
//	double sumReward = 0;
	int position = 0;
	int prevPosition = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;
	
	void readState (std::vector<float>& state);
	float getPositionFromState (const std::vector<float>& state);

};

#endif /* QuadrocopterDiscreteCtrl_hpp */
