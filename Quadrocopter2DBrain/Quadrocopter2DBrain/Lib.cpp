//
//  Lib.cpp
//  QuadrocopterBrain
//
//  Created by anton on 21/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "Lib.hpp"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>

void Lib::randomize () {
    std::srand ((unsigned int)time(NULL));
}

int Lib::randInt (int min, int max) {
    return min + (std::rand() % (int)(max - min + 1));
}

long Lib::randLong (long min, long max) {
	return min + (std::rand() % (long)(max - min + 1));
}

float Lib::randFloat (float LO, float HI) {
    return LO + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

double Lib::randDouble (double LO, double HI) {
    return LO + static_cast <double> (std::rand()) /( static_cast <double> (RAND_MAX/(HI-LO)));
}

long long Lib::getTimestampInMillis () {
    using namespace std::chrono;
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}

std::vector<long> Lib::getRandomNonRepeatSeries (
	long size,
	long min,
	long max
) {
	std::vector<long> series;
	for (int i=0; i<size; i++) {
		while (true) {
			long newItem = randLong (min, max);
			if (std::find(series.begin(), series.end(), newItem) == series.end()) {
				series.push_back (newItem);
				break;
			}
		}
	}
	return series;
};
