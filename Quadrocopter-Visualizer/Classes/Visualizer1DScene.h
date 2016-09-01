#ifndef __Visualizer1DScene_H__
#define __Visualizer1DScene_H__

#include "cocos2d.h"
#include "QuadrocopterSimulator.hpp"
#include "main.h"

class Visualizer1DScene : public cocos2d::Layer
{
public:
	
//	QuadrocopterSimulatorDiscrete sim;
//	QuadrocopterSimulator sim;
	QuadrocopterSimulatorCont1D sim;
	
	std::vector<cocos2d::LayerColor*> qcopterNodes;
	cocos2d::Vec2 centerPos;

	Visualizer1DScene ();
	
	void reset ();

    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void update(float delta) override;
	
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Visualizer1DScene);
};

#endif // __Visualizer1DScene_H__
