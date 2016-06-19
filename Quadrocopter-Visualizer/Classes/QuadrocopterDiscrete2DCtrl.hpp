//
//  QuadrocopterDiscrete2DCtrl.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 18/06/16.
//
//

#ifndef QuadrocopterDiscrete2DCtrl_hpp
#define QuadrocopterDiscrete2DCtrl_hpp

#include "Simulation.hpp"

class QuadrocopterDiscreteModel2D : public QuadrocopterModel2DIFace {
public:
	
	float posX;
	float posY;
	float angle;
	float motor1power;
	float motor2power;
	
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
};

class QuadrocopterDiscrete2DCtrl {
public:
	
	QuadrocopterDiscrete2DCtrl (int id, Quadrocopter2D& simulationModel);
	
	void act ();
	void storeExperience ();
	void calcReward ();
	void reset ();
	double getReward ();
	QuadrocopterModel2DIFace& getModel ();

private:

	int id;
	long action = -1;
//	int timeReward = 0;
	double reward;
//	double sumReward = 0;
	std::vector<float> prevState;
	std::vector<float> nextState;
	
	QuadrocopterDiscreteModel2D model;
	
	void readState (std::vector<float>& state);

//	Quadrocopter2D& simulationModel;

};

#endif /* QuadrocopterDiscrete2DCtrl_hpp */
