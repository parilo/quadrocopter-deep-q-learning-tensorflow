//
//  QuadrocopterDiscrete2DCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 18/06/16.
//
//

#ifndef QuadrocopterDiscrete2DCtrl_hpp
#define QuadrocopterDiscrete2DCtrl_hpp

#include "Simulation.hpp"
#include "WorldDiscrete.hpp"
#include "QuadrocopterDiscrete2D.hpp"

class QuadrocopterDiscrete2DCtrl {
public:
	
	QuadrocopterDiscrete2DCtrl (int id, QuadrocopterDiscrete2D& simulationModel);
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	void resetAction ();
	double getReward ();
	QuadrocopterModel2DIFace& getModel ();

private:

	int id;
	long action = -1;
//	int timeReward = 0;
	double reward;
//	double sumReward = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;
	
	QuadrocopterDiscrete2D& model;
	
	void readState (std::vector<float>& state);

//	Quadrocopter2D& simulationModel;

};

#endif /* QuadrocopterDiscrete2DCtrl_hpp */
