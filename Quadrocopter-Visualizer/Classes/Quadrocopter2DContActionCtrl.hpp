//
//  Quadrocopter2DContActionCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 14/08/16.
//
//

#ifndef Quadrocopter2DContActionCtrl_hpp
#define Quadrocopter2DContActionCtrl_hpp

#include "Quadrocopter2DCtrl.hpp"

class Quadrocopter2DContActionCtrl : public Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DContActionCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	
protected:

	std::vector<float> actionCont;
	
};

#endif /* Quadrocopter2DContActionCtrl_hpp */
