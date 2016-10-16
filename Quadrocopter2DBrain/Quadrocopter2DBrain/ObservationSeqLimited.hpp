//
//  ObservationSeqLimited.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 12/03/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ObservationSeqLimited_hpp
#define ObservationSeqLimited_hpp

#include "Observation.hpp"
#include "Tensors.hpp"

/**
	Observations sequence. FIFO
*/
class ObservationSeqLimited {

public:

	ObservationSeqLimited ();
	ObservationSeqLimited (int limit);
	ObservationSeqLimited (const std::vector <float>& data);

	void setLimit (int limit);
	void push (const Observation& ob);
	int getSize () const;
	void initWith (const Observation& ob);
	const Observation& getObservation (int index) const;
	void print () const;

	friend void fillTensor (const ObservationSeqLimited& obs, tensorflow::Tensor& t, int tensorIndex);
	friend void fillTensor3D (const ObservationSeqLimited& obs, tensorflow::Tensor& t, int tensorIndex);
	
private:
	
	int limit = 0;
	int newItemPos = 0;
	std::vector<Observation> observations;

};

#endif /* ObservationSeqLimited_hpp */
