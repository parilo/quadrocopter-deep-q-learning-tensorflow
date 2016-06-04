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
#include "QuadrocopterDiscreteSimulator.hpp"
#include "main.h"

class Quadrocopter2DView {
public:
	
	void init ();
	void addTo (cocos2d::Node* parent);
	void setCoordsFrom (const Quadrocopter2D& model, cocos2d::Vec2 centerPos, float visualizerZoom);

private:

	float zoom = 50;
	cocos2d::LayerColor* body;
	cocos2d::LayerColor* motor1;
	cocos2d::LayerColor* motor2;
	
	cocos2d::Vec2 bodyHalfCont;
	cocos2d::Vec2 motor1HalfCont;
	cocos2d::Vec2 motor2HalfCont;
};

class Visualizer2DScene : public cocos2d::Layer
{
public:
	
	QuadrocopterSimulator2D sim;
	
	std::vector<Quadrocopter2DView> qcopterViews;
	cocos2d::Vec2 centerPos;

	Visualizer2DScene ();
	
	void reset ();

    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void update(float delta) override;
	
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Visualizer2DScene);
};

#endif /* Visualizer2DScene_hpp */
