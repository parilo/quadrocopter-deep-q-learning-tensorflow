//
//  ObservationSeqLimited.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 12/03/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ObservationSeqLimited.hpp"

ObservationSeqLimited::ObservationSeqLimited () {}

ObservationSeqLimited::ObservationSeqLimited (int limit) {
	setLimit(limit);
}

ObservationSeqLimited::ObservationSeqLimited (const std::vector <float>& data) {
	setLimit(1);
	push(Observation(data));
}

void ObservationSeqLimited::setLimit (int limit) {
	this->limit = limit;
	observations.resize(limit);
}

void ObservationSeqLimited::initWith (const Observation& ob) {
	for (int i=0; i<limit; i++) {
		push (ob);
	}
}

void ObservationSeqLimited::push (const Observation& ob) {
//std::cerr << "push: " << newItemPos << std::endl;
	observations [newItemPos] = ob;
	newItemPos++;
	newItemPos %= limit;
}

int ObservationSeqLimited::getSize () const {
	if (limit == 0) return 0;
	else return limit * observations.front().getSize();
}

void fillTensor (const ObservationSeqLimited& obs, tensorflow::Tensor& t, int tensorIndex) {
	int observationSize = obs.observations.front().getSize();
	
	int obIndex = obs.newItemPos;
	int datai = 0;
	for (int obFilled = 0; obFilled<obs.limit; obFilled++) {
//std::cerr << "fill: obIndex: " << obIndex << " datai: " << datai << std::endl;
		auto& ob = obs.observations [obIndex];
	
		for (int i=0; i<observationSize; i++) {
			t.matrix<float>()(tensorIndex, datai++) = ob.data [i];
//std::cerr << "fill: data: " << ob.data [i] << std::endl;
		}
		
		obIndex++;
		obIndex %= obs.limit;
	}
}

void fillTensor3D (const ObservationSeqLimited& obs, tensorflow::Tensor& t, int tensorIndex) {
	int obsS = obs.observations.size ();
	int obS = obs.observations.front().getSize();
	
	int obIndex = obs.newItemPos;
	for (int obsI=0; obsI < obsS; obsI++) {
		auto& ob = obs.observations [obIndex];
		for (int obI=0; obI < obS; obI++) {
			t.tensor<float, 3>()(tensorIndex, obsI, obI) = ob.data[obI];
		}
		
		obIndex++;
		obIndex %= obs.limit;
	}
}

void ObservationSeqLimited::print () const {
	int obsS = observations.size ();
	int obS = observations.front().getSize();
	
	int obIndex = newItemPos;
	for (int obsI=0; obsI < obsS; obsI++) {
		auto& ob = observations [obIndex];
		for (int obI=0; obI < obS; obI++) {
//			t.tensor<float, 3>()(tensorIndex, obsI, obI) = ob.data[obI];
			std::cout << ob.data[obI] << " ";
		}
		std::cout << "." << std::endl;
		
		obIndex++;
		obIndex %= limit;
	}
	std::cout << std::endl;
}

const Observation& ObservationSeqLimited::getObservation (int index) const {
	return observations [index];
}
