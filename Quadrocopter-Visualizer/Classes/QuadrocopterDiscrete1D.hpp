//
//  QuadrocopterDiscrete1D.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/06/16.
//
//

#ifndef QuadrocopterDiscrete1D_hpp
#define QuadrocopterDiscrete1D_hpp

#include "WorldDiscrete.hpp"

class QuadrocopterDiscrete {

public:

	QuadrocopterDiscrete ();
	virtual ~QuadrocopterDiscrete ();
	
	void createIn (WorldDiscrete1D& w);

	float getPosition ();
	void setPosition (float pos);
	void setVelocity (float v);
	float getVelocity ();
	void setMotorPower (float p);
	
	void step ();
	
private:
	float position;
};

#endif /* QuadrocopterDiscrete1D_hpp */
