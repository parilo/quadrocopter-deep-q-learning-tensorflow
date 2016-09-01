//
//  Obstacle2D.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 24/06/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Obstacle2D_hpp
#define Obstacle2D_hpp

#include "ObstacleModelIFace.hpp"
#include "World.hpp"
#include "WorldObjectInfo.hpp"

class Obstacle2D : public ObstacleModel2DIFace, public WorldObject {
public:

	void createIn (World2D& w);

	void step () {}
	void reset ();
	
	void getCoords (
		float& centerX,
		float& centerY,
		float& a,
		float& b,
		float& angle
	) const override;

	void getPoints (
		float& x0,
		float& y0,
		float& x1,
		float& y1,
		float& x2,
		float& y2,
		float& x3,
		float& y3
	) const override;
	
	bool containsPoint (const b2Vec2& point);

private:

	b2Body* body;
	World2D* world;
	std::shared_ptr<WorldObjectInfo> worldObjectInfo;
	
	float a;
	float b;
	
};

#endif /* Obstacle2D_hpp */
