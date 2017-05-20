//
//  Quadrocopter2DContActionLSTMCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 05/11/2016.
//
//

#ifndef Quadrocopter2DContActionLSTMCtrl_hpp
#define Quadrocopter2DContActionLSTMCtrl_hpp

#include "Quadrocopter2DCtrl.hpp"

class Quadrocopter2DContActionLSTMCtrl : public Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DContActionLSTMCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	void reset () override;
	void resetAction () override;
	
protected:

	std::vector<float> actionCont;
	
	
};

#endif /* Quadrocopter2DContActionLSTMCtrl_hpp */
