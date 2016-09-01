//
//  QuadrocopterContActionCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 13/08/16.
//
//

#ifndef QuadrocopterContActionCtrl_hpp
#define QuadrocopterContActionCtrl_hpp

#include "QuadrocopterCtrl.hpp"

class QuadrocopterContActionCtrl : public QuadrocopterCtrl {
public:
	
	QuadrocopterContActionCtrl (int id, Quadrocopter1D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	void onSimulationStep (int stepIndex) override;
	
protected:

	std::vector<float> actionCont;
	
	
};

#endif /* QuadrocopterContActionCtrl_hpp */
