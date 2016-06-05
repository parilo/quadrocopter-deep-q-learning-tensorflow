//
//  Visualizer2DScene.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/05/16.
//
//

#include <iostream>

#include "Visualizer2DScene.hpp"
#include "Quadrocopter2DBrain.hpp"

USING_NS_CC;

Visualizer2DScene::Visualizer2DScene () : sim() {}

Scene* Visualizer2DScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Visualizer2DScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Visualizer2DScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Visualizer2DScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	centerPos = getContentSize() * 0.5;

	LayerColor* targetNode1 = LayerColor::create(Color4B::ORANGE);
	targetNode1->setAnchorPoint(Vec2(0.5, 0.5));
	targetNode1->setContentSize(Size(2, 80));
	targetNode1->setPosition (centerPos - targetNode1->getContentSize()/2);
	addChild(targetNode1);

	LayerColor* targetNode2 = LayerColor::create(Color4B::ORANGE);
	targetNode2->setAnchorPoint(Vec2(0.5, 0.5));
	targetNode2->setContentSize(Size(80, 2));
	targetNode2->setPosition (centerPos - targetNode2->getContentSize()/2);
	addChild(targetNode2);
	
	sim.init();

	for (int i=0; i<quadrocoptersCount; i++) {
	
		Quadrocopter2DView view;
		view.init();
		view.addTo(this);
		qcopterViews.push_back (view);
	}

//	reset ();
//	scheduleUpdate();

	sim.setSimulationUpdateCallback([this](){
		update (0);
	});
	sim.startActWorkers();
//	sim.startTrainWorkers();
	
    return true;
}

void Visualizer2DScene::update(float delta) {

//	sim.update();

	for (int i=0; i<quadrocoptersCount; i++) {
		Quadrocopter2DView& view = qcopterViews [i];
		view.setCoordsFrom(sim.getQuadrocopterCtrl(i).getSimulationModel(), centerPos, 4);
	}
	
}

void Visualizer2DScene::reset () {
	sim.reset();
}



void Visualizer2DScene::menuCloseCallback(Ref* pSender)
{
	sim.stop();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Quadrocopter2DView::init () {
	body = LayerColor::create(Color4B(0, 0, 255, 255));
	body->setAnchorPoint(Vec2(0.5, 0.5));
	body->setContentSize(Size(0.4, 0.1) * zoom);
	bodyHalfCont = body->getContentSize()/2;

	motor1 = LayerColor::create(Color4B(0, 255, 0, 255));
	motor1->setAnchorPoint(Vec2(0.5, 0.5));
	motor1->setContentSize(Size(0.1, 0.2) * zoom);
	motor1HalfCont = motor1->getContentSize()/2;

	motor2 = LayerColor::create(Color4B(255, 0, 0, 255));
	motor2->setAnchorPoint(Vec2(0.5, 0.5));
	motor2->setContentSize(Size(0.1, 0.2) * zoom);
	motor2HalfCont = motor2->getContentSize()/2;
}

void Quadrocopter2DView::addTo (cocos2d::Node* parent) {
	parent->addChild(body);
	parent->addChild(motor1);
	parent->addChild(motor2);
}

void Quadrocopter2DView::setCoordsFrom (
	const Quadrocopter2D& model,
	cocos2d::Vec2 centerPos,
	float visualizerZoom
) {
	b2Vec2 bodyPos;
	b2Vec2 motor1Pos;
	b2Vec2 motor2Pos;
	float bodyRotation;
	float motor1Rotation;
	float motor2Rotation;
	
	model.getPartsCoords (
		bodyPos,
		motor1Pos,
		motor2Pos,
		bodyRotation,
		motor1Rotation,
		motor2Rotation
	);

//	body->setPosition(Vec2(bodyPos.x, -bodyPos.y) - bodyHalfCont);
//	body->setRotation(bodyRotation);
//	motor1->setPosition(Vec2(motor1Pos.x, -motor1Pos.y) - motor1HalfCont);
//	motor1->setRotation(motor1Rotation);
//	motor2->setPosition(Vec2(motor2Pos.x, -motor2Pos.y) - motor2HalfCont);
//	motor2->setRotation(motor2Rotation);

//	visualizerZoom = 1;
//	
//	body->setPosition(Vec2 (bodyPos.x, bodyPos.y));
//	body->setRotation(bodyRotation);
//
//	Vec2 m1pos = body->convertToWorldSpace(body->convertToNodeSpace(Vec2(motor1Pos.x, motor1Pos.y)) * 1 / visualizerZoom);
//	Vec2 m2pos = body->convertToWorldSpace(body->convertToNodeSpace(Vec2(motor2Pos.x, motor2Pos.y)) * 1 / visualizerZoom);
//
//	body->setPosition(body->getPosition() * visualizerZoom - bodyHalfCont + centerPos);
//	motor1->setPosition(m1pos * visualizerZoom - motor1HalfCont + centerPos);
//	motor1->setRotation(motor1Rotation);
//	motor2->setPosition(m2pos * visualizerZoom - motor2HalfCont + centerPos);
//	motor2->setRotation(motor2Rotation);



	Vec2 bPos (bodyPos.x, -bodyPos.y);
	Vec2 m1pos = (Vec2(motor1Pos.x, -motor1Pos.y) - bPos) * zoom / visualizerZoom + bPos;
	Vec2 m2pos = (Vec2(motor2Pos.x, -motor2Pos.y) - bPos) * zoom / visualizerZoom + bPos;

	body->setPosition(bPos * visualizerZoom - bodyHalfCont + centerPos);
	body->setRotation(bodyRotation);
	motor1->setPosition(m1pos * visualizerZoom - motor1HalfCont + centerPos);
	motor1->setRotation(motor1Rotation);
	motor2->setPosition(m2pos * visualizerZoom - motor1HalfCont + centerPos);
	motor2->setRotation(motor2Rotation);
	
	float p1;
	float p2;
	model.getMotorPower(p1, p2);
	motor1->setColor(Color3B(0, p1 > 0?255:64, 0));
	motor2->setColor(Color3B(p2 > 0?255:64, 0, 0));
}
