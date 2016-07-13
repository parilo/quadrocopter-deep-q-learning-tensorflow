//
//  QuadrocopterDiscrete2D.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 23/06/16.
//
//

#ifndef QuadrocopterDiscrete2D_hpp
#define QuadrocopterDiscrete2D_hpp

#include "WorldDiscrete.hpp"
#include "QuadrocopterModelIFace.hpp"

class QuadrocopterDiscrete2D : public QuadrocopterModel2DIFace {
public:
	
	QuadrocopterDiscrete2D ();
	void createIn (WorldDiscrete2D& w);
	void step ();

	float posX;
	float posY;
	float angle;
	float motor1power;
	float motor2power;
	float targetX = 0;
	float targetY = 0;
	
	virtual void setTarget (const b2Vec2& pos) override;
	virtual void setCoords (const b2Vec2& pos, float angle) override;
	virtual void setVelocity (const b2Vec2& v) override;
	virtual void setAngularVelocity (float w) override;
	
	virtual void getPartsCoords (
		b2Vec2& bodyPos,
		b2Vec2& motor1Pos,
		b2Vec2& motor2Pos,
		float& bodyRotation,
		float& motor1Rotation,
		float& motor2Rotation
	) const override;

	virtual void getMotorPower (float& p1, float& p2) const override;

protected:

	void getMainCoords (
		float& posX,
		float& posY,
		float& angle
	) const override;

private:

};

#endif /* QuadrocopterDiscrete2D_hpp */
