//
//  QuadrocopterModelIFace.cpp
//  Quadrocopter-Box2D
//
//  Created by anton on 18/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "QuadrocopterModelIFace.hpp"

void QuadrocopterModel2DIFace::setId (int id)
{
	this->id = id;
}

int QuadrocopterModel2DIFace::getId () const
{
	return id;
}

void QuadrocopterModel2DIFace::setCollided (bool collided) {
	this->collided =collided;
	for (auto& s : getSensors()) {
		s = 0;
	}
}

bool QuadrocopterModel2DIFace::isCollided () const {
	return collided;
}
