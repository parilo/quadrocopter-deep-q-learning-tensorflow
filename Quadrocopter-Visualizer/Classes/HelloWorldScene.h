#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "QuadrocopterSimulator.hpp"
#include "main.h"

class HelloWorld : public cocos2d::Layer
{
public:
	
//	QuadrocopterSimulatorDiscrete sim;
	QuadrocopterSimulator sim;
	
	std::vector<cocos2d::LayerColor*> qcopterNodes;
	cocos2d::Vec2 centerPos;

	HelloWorld ();
	
	void reset ();

    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void update(float delta) override;
	
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
