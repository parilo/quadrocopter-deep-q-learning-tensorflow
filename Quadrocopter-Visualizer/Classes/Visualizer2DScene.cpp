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
#include "Lib.h"

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
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
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

	targetNode1 = LayerColor::create(Color4B::ORANGE);
	targetNode1->setAnchorPoint(Vec2(0.5, 0.5));
	targetNode1->setContentSize(cocos2d::Size(2, 80));
	targetNode1->setPosition (centerPos - targetNode1->getContentSize()/2);
	addChild(targetNode1);

	targetNode2 = LayerColor::create(Color4B::ORANGE);
	targetNode2->setAnchorPoint(Vec2(0.5, 0.5));
	targetNode2->setContentSize(cocos2d::Size(80, 2));
	targetNode2->setPosition (centerPos - targetNode2->getContentSize()/2);
	addChild(targetNode2);
	
	sim.init();

	for (int i=0; i<obstaclesCount; i++)
	{
		Obstacle2DView view;
		view.init();
		view.addTo(this);
		obstacleViews.push_back (view);
	}

	for (int i=0; i<quadrocoptersCount; i++)
	{
		Quadrocopter2DView view;
		view.init();
		view.addTo(this);
		qcopterViews.push_back (view);
	}
	
//	for (int i=0; i<debugPointsX; i++) {
//		for (int j=0; j<debugPointsY; j++) {
//			LayerColor* dp = LayerColor::create(Color4B::BLUE, 5, 5);
//			dp->setPosition(Vec2(i*4 - 100, 100 - j*4)*4 + centerPos);
//			addChild(dp);
//			debugPoints.push_back(dp);
//		}
//	}
	
	changeTarget ();

	sim.setSimulationUpdateCallback([this](){
		update (0);
		
		if (simulationStep > 0)
		if (simulationStep % changeTargetInSteps == 0) {
			changeTarget ();
		}
		
		simulationStep++;
	});
	
//	sim.setCollideListener([this](ObstacleModel2DIFace& o, QuadrocopterModel2DIFace& q){
//
//		float centerX;
//		float centerY;
//		float a;
//		float b;
//		float angle;
//
//		o.getCoords(
//			centerX,
//			centerY,
//			a,
//			b,
//			angle
//		);
//	
//		b2Vec2 bodyPos;
//		b2Vec2 motor1Pos;
//		b2Vec2 motor2Pos;
//		float bodyRotation;
//		float motor1Rotation;
//		float motor2Rotation;
//		
//		q.getPartsCoords (
//			bodyPos,
//			motor1Pos,
//			motor2Pos,
//			bodyRotation,
//			motor1Rotation,
//			motor2Rotation
//		);
//	
//		CCLOG ("--- collide o: %f %f %f %f %f q: %d %f %f",
//			centerX,
//			centerY,
//			a,
//			b,
//			angle,
//			q.getId(),
//			bodyPos.x,
//			bodyPos.y
//		);
//	
//	});
	
	sim.startActWorkers();
	sim.startTrainWorkers();
	
    return true;
}

void Visualizer2DScene::update(float delta) {

	for (int i=0; i<quadrocoptersCount; i++) {
		qcopterViews [i].setCoordsFrom(sim.getQuadrocopterCtrl(i).getModel(), centerPos, 4);
	}

	for (int i=0; i<obstaclesCount; i++) {
		obstacleViews [i].setCoordsFrom(sim.getObstacle(i), centerPos, 4);
	}

//	QuadrocopterDiscrete2D qq;
//	for (int i=0; i<debugPointsX; i++) {
//		for (int j=0; j<debugPointsY; j++) {
//			LayerColor* dp = debugPoints [i*debugPointsX + j];
//			Vec2 pos (i*4 - 100, j*4 - 100);
//			for (int oi=0; oi<obstaclesCount; oi++) {
//				qq.setCoords(b2Vec2(pos.x, pos.y), 0);
//				if (sim.getObstacle(oi).isCollidedWith(qq)) {
//					dp->setColor(Color3B::RED);
//					break;
//				} else {
//					dp->setColor(Color3B::BLUE);
//				}
//			}
//		}
//	}
	
}

void Visualizer2DScene::changeTarget() {
	
	b2Vec2 newTarget;
	do {
		newTarget = b2Vec2(Lib::randFloat(-70, 70), Lib::randFloat(-70, 70));
	} while (sim.getWorld().isPointInsideObstacles (newTarget));
	
	for (int i=0; i<quadrocoptersCount; i++) {
		sim.getQuadrocopterCtrl(i).getModel().setTarget(newTarget);
		sim.getQuadrocopterCtrl(i).resetAction();
	}
	targetNode1->setPosition (centerPos - targetNode1->getContentSize()/2 + 4*Vec2(newTarget.x, -newTarget.y));
	targetNode2->setPosition (centerPos - targetNode2->getContentSize()/2 + 4*Vec2(newTarget.x, -newTarget.y));
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
//	body->setAnchorPoint(Vec2(0.5, 0.5));
	body->setContentSize(cocos2d::Size(0.4, 0.1) * zoom);
	bodyHalfCont = body->getContentSize()/2;

	motor1 = LayerColor::create(Color4B(0, 255, 0, 255));
	motor1->setAnchorPoint(Vec2(0.5, 0.5));
	motor1->setContentSize(cocos2d::Size(0.1, 0.2) * zoom);
	motor1HalfCont = motor1->getContentSize()/2;

	motor2 = LayerColor::create(Color4B(255, 0, 0, 255));
	motor2->setAnchorPoint(Vec2(0.5, 0.5));
	motor2->setContentSize(cocos2d::Size(0.1, 0.2) * zoom);
	motor2HalfCont = motor2->getContentSize()/2;
	
	sensors = DrawNode::create();
//	sensors->setAnchorPoint(Vec2(0.5, 0.5));
	sensors->setContentSize(cocos2d::Size(QuadrocopterModel2DIFace::sensorsLength * 2 * 4, QuadrocopterModel2DIFace::sensorsLength * 2 * 4));
}

void Quadrocopter2DView::addTo (cocos2d::Node* parent) {
	parent->addChild(sensors);
	parent->addChild(body);
	parent->addChild(motor1);
	parent->addChild(motor2);
}

void Quadrocopter2DView::setCoordsFrom (
	const QuadrocopterModel2DIFace& model,
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
	motor1->setColor(Color3B(0, 64 + p1 * (255-64) / 20.0, 0));
	motor2->setColor(Color3B(64 + p2 * (255-64) / 20.0, 0, 0));
	
	if (model.isCollided ()) {
		body->setColor(Color3B::RED);
	} else {
		body->setColor(Color3B::BLUE);
	}
	
	//sensors
	sensors->setPosition(body->getPosition() + bodyHalfCont);
	sensors->setRotation(body->getRotation() + 180);
	sensors->clear();
	Vec2 origin (0, 0);
//	float angle = bodyRotation * M_PI / 180 - M_PI_2;

	std::vector<float>& sensorsData = model.getSensors ();
	for (int i=0; i<QuadrocopterModel2DIFace::sensorsCount; i++) {
		float m = sensorsData [i] * visualizerZoom * QuadrocopterModel2DIFace::sensorsLength / QuadrocopterModel2DIFace::sensorsMagnitude;
		Vec2 S (
			m * sinf (i * 2 * M_PI / QuadrocopterModel2DIFace::sensorsCount),
			- m * cosf (i * 2 * M_PI / QuadrocopterModel2DIFace::sensorsCount)
		);
		
		float sensorValue = sensorsData [i] / QuadrocopterModel2DIFace::sensorsMagnitude;
//		if (sensorValue < 1 || i==0) {
//			sensors->drawLine(origin, S, i==0?Color4F::RED:Color4F(Color4B((1-sensorValue) * 255, 0, sensorValue * 255, 200)));
//		}
		if (sensorValue < 1) {
			sensors->drawLine(origin, S, Color4F(Color4B((1-sensorValue) * 255, 0, sensorValue * 255, 200)));
		}
	}
	
}



void Obstacle2DView::init () {
	body = LayerColor::create(Color4B(0, 255, 255, 128));
	body->setAnchorPoint(Vec2(0.5, 0.5));
}

void Obstacle2DView::addTo (cocos2d::Node* parent) {
	parent->addChild(body);
}

void Obstacle2DView::setCoordsFrom (
	const ObstacleModel2DIFace& model,
	cocos2d::Vec2 centerPos,
	float visualizerZoom
) {

	float centerX;
	float centerY;
	float a;
	float b;
	float angle;

	model.getCoords(
		centerX,
		centerY,
		a,
		b,
		angle
	);

	Vec2 bPos (centerX, -centerY);
	
	body->setContentSize(cocos2d::Size(a, b) * visualizerZoom);
	body->setPosition(bPos * visualizerZoom - body->getContentSize() * 0.5 + centerPos);
	body->setRotation(angle);
}
