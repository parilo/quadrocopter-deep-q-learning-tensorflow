//
//  Observation.hpp
//  QuadrocopterBrain
//
//  Created by anton on 21/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Observation_hpp
#define Observation_hpp

#include <vector>

class Observation {

public:

	Observation ();

	Observation (
		const std::vector<float>& data
	);

	void set (
		const std::vector<float>& data
	);
	
	void setZeros (int len);

	std::vector<float> data;
	
	int getSize () const;
	void print () const;

};

#endif /* Observation_hpp */
