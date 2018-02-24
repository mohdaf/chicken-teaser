#include "HelloWorldScene.h"
#include "Definitions.h"
#include "MainMenuScene.h"
#include "MainMenuScene.cpp"

USING_NS_CC;

Scene* HelloWorldScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorldScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorldScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->scheduleOnce(schedule_selector(HelloWorldScene::GoToMainMenuScene),
			SPLASH_INTERVAL);

	float w = visibleSize.width;
	float h = visibleSize.height;

	Sprite * bgsprite = Sprite::create("images/semicolonlogo.png");
	bgsprite->setPosition(Point(w / 2 + origin.x, h / 2 + origin.y));
	bgsprite->setScale(w / bgsprite->getContentSize().width,
			h / bgsprite->getContentSize().height);
	this->addChild(bgsprite);

	return true;
}


void HelloWorldScene::GoToMainMenuScene(float dt) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(
			TransitionFade::create(TRANSACTION_TIME, scene));

}

