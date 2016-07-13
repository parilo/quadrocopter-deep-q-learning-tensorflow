//
//  Lib.hpp
//  QuadrocopterBrain
//
//  Created by anton on 21/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Lib_hpp
#define Lib_hpp

#include <vector>

class Lib {

public:
	
	static void randomize ();
	static int randInt (int min, int max);
	static long randLong (long min, long max);
	static float randFloat (float LO, float HI);
	static double randDouble (double LO, double HI);
	static long long getTimestampInMillis ();
	
	static std::vector<long> getRandomNonRepeatSeries (
		long size,
		long min,
		long max
	);

	template <typename T>
	static void getRandomSubArray (
		const std::vector<T>& allItems,
		std::vector<const T*>& subArray,
		long subArrayLength
	) {
		std::vector<long> indices = Lib::getRandomNonRepeatSeries(subArrayLength, 0, allItems.size()-1);
		for (auto index : indices) {
			subArray.push_back(&allItems[index]);
		}
	}
	
	template <typename T>
	static int argmax (const std::vector<T>& items) {
		int maxIndex = 0;
		T maxVal = items [maxIndex];
		int index = 0;
		for (auto& i : items) {
			if (i > maxVal) {
				maxVal = i;
				maxIndex = index;
			}
			index++;
		}
		return maxIndex;
	}
	
	template <typename T>
	static int argmaxabs (const std::vector<T>& items) {
		int maxIndex = 0;
		T maxVal = fabs(items [maxIndex]);
		int index = 0;
		for (auto& i : items) {
			float fabsi = fabs(i);
			if (fabsi > maxVal) {
				maxVal = fabsi;
				maxIndex = index;
			}
			index++;
		}
		return maxIndex;
	}
	
};

#endif /* Lib_hpp */
