#include <iostream>

#include "Visualizer1DScene.h"
#include "Quadrocopter2DBrain.hpp"

USING_NS_CC;

Visualizer1DScene::Visualizer1DScene () : sim() {}

Scene* Visualizer1DScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Visualizer1DScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Visualizer1DScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Visualizer1DScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	centerPos = getContentSize() * 0.5;

	LayerColor* targetNode = LayerColor::create(Color4B::ORANGE);
	targetNode->setAnchorPoint(Vec2(0.5, 0.5));
	targetNode->setContentSize(cocos2d::Size(2, 80));
	targetNode->setPosition (centerPos);
	addChild(targetNode);
	
	sim.init();
	
//	scheduleUpdate();

	for (int i=0; i<quadrocoptersCount; i++) {
	
		LayerColor* qcopterNode = LayerColor::create(Color4B(0, 0, 255, 255));
		qcopterNodes.push_back(qcopterNode);
		qcopterNode->setAnchorPoint(Vec2(0.5, 0.5));
		qcopterNode->setContentSize(cocos2d::Size(2, 20));
		qcopterNode->setPosition (centerPos);
		addChild(qcopterNode);
	}

	reset ();
	
	sim.setSimulationUpdateCallback([this](){
		update (0);
	});
	sim.startActWorkers();
	sim.startTrainWorkers();
	
    return true;
}

void Visualizer1DScene::update(float delta) {

//	sim.update();

	for (int i=0; i<quadrocoptersCount; i++) {
		LayerColor* qcopterNode = qcopterNodes [i];
		float pos = 4 * sim.getQuadrocopterCtrl(i).getModel().getPosition();
		qcopterNode->setPosition(centerPos + Vec2 (pos, 0));
	}
	
}

void Visualizer1DScene::reset () {
	sim.reset();
}



void Visualizer1DScene::menuCloseCallback(Ref* pSender)
{
	sim.stop();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
