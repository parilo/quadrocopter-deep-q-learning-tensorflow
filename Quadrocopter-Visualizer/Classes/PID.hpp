//
//  PID.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 04/09/16.
//
//

#ifndef PID_hpp
#define PID_hpp

class PID {
public:
	
	PID (double P, double I, double D);

	double calc (double current, double target);

private:
	double P;
	double I;
	double D;
	
	double prevErr;
	double sumErr;
	
};

#endif /* PID_hpp */
