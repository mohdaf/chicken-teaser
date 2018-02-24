#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameScene.cpp"
#include "Definitions.h"
#include "AdmobHelper.h"
#include "AdmobHelper.cpp"

USING_NS_CC;
//MainMenuScene::onEnter() {
//	UserDefault *def = UserDefault::getInstance();
//	auto highscore = def->getIntegerForKey("HIGHSCORE");
//	__String *tempscore2 = __String::createWithFormat("BEST : %i", highscore);
//	bestScoreLabel->setString(tempscore2->getCString());
//}
Scene* MainMenuScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init() {

	if (!Layer::init()) {
		return false;
	}
	// Sound
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(
				"sounds/game1.mp3");
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
				"sounds/button.mp3");
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(
				0.2f);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float w = visibleSize.width;
	float h = visibleSize.height;

	Sprite * bgsprite = cocos2d::Sprite::create("images/start.png");
	bgsprite->setPosition(Point(w / 2 + origin.x, h / 2 + origin.y));
	bgsprite->setScale(w / bgsprite->getContentSize().width,
			h / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

	MenuItemImage *playItem = MenuItemImage::create("images/play1.png",
			"images/play2.png",
			CC_CALLBACK_1(MainMenuScene::GoToGameScene,this));
	float scale = (w / playItem->getContentSize().width) * 0.55f;
	playItem->setScale(scale);
	Size s1 = playItem->getBoundingBox().size;
	playItem->setPosition(
			Point(w / 3 - s1.width, h * 3 / 4 - s1.height * 6 / 4));

	MenuItemImage * multiPlayerItem = MenuItemImage::create(
			"images/multiplayer1.png", "images/multiplayer2.png",
			CC_CALLBACK_1(MainMenuScene::Multi_player,this));
	scale = (w / multiPlayerItem->getContentSize().width) * 0.55f;
	multiPlayerItem->setScale(scale);
	Size s2 = multiPlayerItem->getBoundingBox().size;
	multiPlayerItem->setPosition(
			Point(w / 3 - s2.width, h / 2 - s2.height * 6 / 4));

	MenuItemImage *ExitItem = MenuItemImage::create("images/exit2.png",
			"images/exit3.png", CC_CALLBACK_1(MainMenuScene::Exit,this));
	scale = (w / ExitItem->getContentSize().width) * 0.55f;
	ExitItem->setScale(scale);
	Size s3 = ExitItem->getBoundingBox().size;
	ExitItem->setPosition(Point(w / 3 - s3.width, h / 4 - s3.height * 6 / 4));

	Menu *menu = Menu::create(playItem, multiPlayerItem, ExitItem, NULL);
	menu->setPosition(Point(w / 2, h / 2));

	UserDefault *def = UserDefault::getInstance();
	auto highscore = def->getIntegerForKey("HIGHSCORE");
	__String *tempscore2 = __String::createWithFormat("BEST : %i", highscore);
	bestScoreLabel = Label::createWithTTF(tempscore2->getCString(),
			"fonts/Marker Felt.ttf", h * SCORE_FONT_SIZE);
	bestScoreLabel->setColor(Color3B(184, 154, 74));
	Size s = bestScoreLabel->getBoundingBox().size;
	bestScoreLabel->setPosition(w - s.width / 1.5f, h - s.height / 1.5f);
	this->addChild(bestScoreLabel, 100);

	this->addChild(menu, 1);
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
				"sounds/game.mp3", true);
	return true;
}

void MainMenuScene::GoToGameScene(cocos2d::Ref *sender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"sounds/button.mp3");
	this->scheduleOnce(schedule_selector(MainMenuScene::start), 0.5f);
}
void MainMenuScene::Exit(cocos2d::Ref *sender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"sounds/button.mp3");
	this->scheduleOnce(schedule_selector(MainMenuScene::quite), 0.5f);
}
void MainMenuScene::quite(float dt) {
	Director::getInstance()->end();
}
void MainMenuScene::start(float dt) {
	auto scene = GameScene::createScene();
	Director::getInstance()->pushScene(
			TransitionFade::create(TRANSACTION_TIME, scene));
}
void MainMenuScene::Multi_player(cocos2d::Ref *sender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"sounds/button.mp3");
}

