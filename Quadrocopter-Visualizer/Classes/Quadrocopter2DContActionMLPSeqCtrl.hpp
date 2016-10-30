//
//  Quadrocopter2DContActionMLPSeqCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 26/10/16.
//
//

#ifndef Quadrocopter2DContActionMLPSeqCtrl_hpp
#define Quadrocopter2DContActionMLPSeqCtrl_hpp

#include "Quadrocopter2DCtrl.hpp"

class Quadrocopter2DContActionMLPSeqCtrl : public Quadrocopter2DCtrl {
public:
	
	Quadrocopter2DContActionMLPSeqCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act () override;
	void calcReward () override;
	void storeExperience () override;
	void reset () override;
	void resetAction () override;
	
protected:

	std::vector<float> actionCont;
	
};

#endif /* Quadrocopter2DContActionMLPSeqCtrl_hpp */
