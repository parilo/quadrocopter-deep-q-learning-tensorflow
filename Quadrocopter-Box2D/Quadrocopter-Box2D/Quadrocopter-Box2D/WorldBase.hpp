//
//  WorldBase.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef WorldBase_hpp
#define WorldBase_hpp

#include <vector>
#include <functional>
#include <iostream>

template <typename QuadrocopterType, typename ObstacleType>
class WorldBase {

public:

	typedef std::function<void (
		ObstacleType& obstacle,
		QuadrocopterType& copter
	)> CollideListener;

	std::vector<QuadrocopterType>& getQuadrocopters ();
	ObstacleType& getObstacle (int i);
	
	std::vector<ObstacleType>& getObstacles () {
		return obstacles;
	}
	
	virtual void step () {
		for (auto& q : quadrocopters) {
			q.step();
		}
		for (auto& o : obstacles) {
			o.step();
		}
	}

	virtual QuadrocopterType& createQuadrocopter ();
	virtual ObstacleType& createObstacle ();
	
	void setCollideListener (CollideListener listener) {
		collideListener = listener;
	}

protected:

	std::vector<QuadrocopterType> quadrocopters;
	std::vector<ObstacleType> obstacles;

	CollideListener collideListener;

};

template <typename QuadrocopterType, typename ObstacleType>
std::vector<QuadrocopterType>& WorldBase<QuadrocopterType, ObstacleType>::getQuadrocopters () {
	return quadrocopters;
}

template <typename QuadrocopterType, typename ObstacleType>
ObstacleType& WorldBase<QuadrocopterType, ObstacleType>::getObstacle (int i) {
	return obstacles [i];
}

template <typename QuadrocopterType, typename ObstacleType>
QuadrocopterType& WorldBase<QuadrocopterType, ObstacleType>::createQuadrocopter () {
	quadrocopters.emplace_back();
	return quadrocopters.back ();
}

template <typename QuadrocopterType, typename ObstacleType>
ObstacleType& WorldBase<QuadrocopterType, ObstacleType>::createObstacle () {
	obstacles.emplace_back();
	return obstacles.back ();
}

#endif /* WorldBase_hpp */
