//
//  WorldObjectInfo.hpp
//  Quadrocopter-Box2D
//
//  Created by anton on 16/08/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef WorldObjectInfo_hpp
#define WorldObjectInfo_hpp

class WorldObjectInfo {
public:

	enum class Type {
		Quadrocopter = 1,
		Obstacle
	};

	WorldObjectInfo (Type t, int id) : type(t), id(id) {}

	Type type;
	int id;

};

class WorldObject {
public:

	void setId (int id) { this->id = id; }
	int getId () { return id; }

private:

	int id;

};

#endif /* WorldObjectInfo_hpp */
