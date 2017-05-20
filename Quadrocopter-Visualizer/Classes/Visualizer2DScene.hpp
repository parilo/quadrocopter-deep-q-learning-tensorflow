//
//  Visualizer2DScene.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/05/16.
//
//

#ifndef Visualizer2DScene_hpp
#define Visualizer2DScene_hpp

#include "cocos2d.h"
#include "QuadrocopterSimulator.hpp"
#include "main.h"

class Quadrocopter2DView {
public:
	
	void init ();
	void addTo (cocos2d::Node* parent);
	void setCoordsFrom (const QuadrocopterModel2DIFace& model, cocos2d::Vec2 centerPos, float visualizerZoom);

private:

	float zoom = 50;
	cocos2d::LayerColor* body;
	cocos2d::LayerColor* motor1;
	cocos2d::LayerColor* motor2;
	cocos2d::DrawNode* sensors;
	
	cocos2d::Vec2 bodyHalfCont;
	cocos2d::Vec2 motor1HalfCont;
	cocos2d::Vec2 motor2HalfCont;
};


class Obstacle2DView {
public:
	
	void init ();
	void addTo (cocos2d::Node* parent);
	void setCoordsFrom (const ObstacleModel2DIFace& model, cocos2d::Vec2 centerPos, float visualizerZoom);

private:

//	float zoom = 50;
	cocos2d::LayerColor* body;
};

class Visualizer2DScene : public cocos2d::Layer
{
public:
	
//	QuadrocopterSimulator2D sim;
//	QuadrocopterSimulatorDiscrete2D sim;
	QuadrocopterSimulatorCont2D sim;
//	QuadrocopterSimulatorContLSTMWeak2D sim;
//	QuadrocopterSimulatorContPID2D sim;
//	QuadrocopterSimulatorContPIDLSTM2D sim;
//	QuadrocopterSimulatorContMLPSeq2D sim;
//	QuadrocopterSimulatorContLSTM2D sim;

	int simulationStep;
	
	std::vector<Quadrocopter2DView> qcopterViews;
	std::vector<Obstacle2DView> obstacleViews;
	cocos2d::Vec2 centerPos;

	const int debugPointsX = 50;
	const int debugPointsY = 50;
	std::vector<cocos2d::LayerColor*> debugPoints;

	cocos2d::LayerColor* targetNode1;
	cocos2d::LayerColor* targetNode2;

	Visualizer2DScene ();
	
	void reset ();
	void changeTarget ();

    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void update(float delta) override;
	
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Visualizer2DScene);
};

#endif /* Visualizer2DScene_hpp */
