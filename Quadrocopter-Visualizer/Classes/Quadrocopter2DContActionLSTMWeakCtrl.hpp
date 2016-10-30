//
//  Quadrocopter2DContActionLSTMWeakCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/10/16.
//
//

#ifndef Quadrocopter2DContActionLSTMWeakCtrl_hpp
#define Quadrocopter2DContActionLSTMWeakCtrl_hpp

#include "Quadrocopter2DCtrl.hpp"

class Quadrocopter2DContActionLSTMWeakCtrl : public Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DContActionLSTMWeakCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	void reset () override;
	void resetAction () override;
	
protected:

	std::vector<float> actionCont;
	
};

#endif /* Quadrocopter2DContActionLSTMWeakCtrl_hpp */
