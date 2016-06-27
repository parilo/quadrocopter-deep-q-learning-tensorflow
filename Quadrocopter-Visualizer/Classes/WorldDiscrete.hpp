//
//  WorldDiscrete.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/06/16.
//
//

#ifndef WorldDiscrete_hpp
#define WorldDiscrete_hpp

template <typename QuadrocopterType, typename ObstacleType>
class WorldDiscrete;
class QuadrocopterDiscrete;
class QuadrocopterDiscrete2D;
class ObstacleDiscrete1D;
class ObstacleDiscrete2D;

typedef WorldDiscrete<QuadrocopterDiscrete, ObstacleDiscrete1D> WorldDiscrete1D;
typedef WorldDiscrete<QuadrocopterDiscrete2D, ObstacleDiscrete2D> WorldDiscrete2D;

#include "World.hpp"
#include "QuadrocopterDiscrete1D.hpp"
#include "QuadrocopterDiscrete2D.hpp"
#include "ObstacleDiscrete1D.hpp"
#include "ObstacleDiscrete2D.hpp"

template <typename QuadrocopterType, typename ObstacleType>
class WorldDiscrete : public WorldBase<QuadrocopterType, ObstacleType> {
public:
	
	void create ();
	void step () override;

private:

};


template <typename QuadrocopterType, typename ObstacleType>
void WorldDiscrete<QuadrocopterType, ObstacleType>::create () {}

template <typename QuadrocopterType, typename ObstacleType>
void WorldDiscrete<QuadrocopterType, ObstacleType>::step () {
	WorldBase<QuadrocopterType, ObstacleType>::step ();
	for (auto& q : WorldBase<QuadrocopterType, ObstacleType>::quadrocopters) {
		q.setCollided (false);
		q.clearSensors ();
		for (auto& o : WorldBase<QuadrocopterType, ObstacleType>::obstacles) {
			q.sense (o);
			if (o.isCollidedWith (q)) {
				q.setCollided (true);
				if (WorldBase<QuadrocopterType, ObstacleType>::collideListener) {
					WorldBase<QuadrocopterType, ObstacleType>::collideListener (o, q);
				}
				break;
			}
		}
	}
}

#endif /* WorldDiscrete_hpp */
