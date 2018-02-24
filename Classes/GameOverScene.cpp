#include "GameOverScene.h"
#include "GameScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* GameOverScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init() {

	if (!Layer::init()) {
		return false;
	}

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/button.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/gameover.mp3");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float w = visibleSize.width;
	float h = visibleSize.height;

	Sprite * bgsprite = Sprite::create("images/game over.png");
	bgsprite->setPosition(Vec2(w / 2, h / 2));
	bgsprite->setScale(w / bgsprite->getContentSize().width,
			h / bgsprite->getContentSize().height);
	this->addChild(bgsprite, -4);

	MenuItemImage *replayItem = MenuItemImage::create("images/replay1.png",
			"images/replay2.png", CC_CALLBACK_1(GameOverScene::Replay,this));
	float scale = (w / replayItem->getContentSize().width) * 0.5f;
	replayItem->setScale(scale);
	Size s3 = replayItem->getBoundingBox().size;
	replayItem->setPosition(Point(w / 4-s3.width, h / 4));

	MenuItemImage *ReturnItem = MenuItemImage::create("images/return1.png",
			"images/return2.png", CC_CALLBACK_1(GameOverScene::ReturnToMainMenu,this));
	scale = (w / ReturnItem->getContentSize().width) * 0.55f;
	ReturnItem->setScale(scale);
	Size s4 = ReturnItem->getBoundingBox().size;
	ReturnItem->setPosition(Point(w *3/ 4-s4.width, h / 4));

	Menu *menu = Menu::create(replayItem, ReturnItem, NULL);
	menu->setPosition(Point(w / 2, h / 2));

	UserDefault *def = UserDefault::getInstance();

	auto score = def->getIntegerForKey("SCORE");
	__String *tempscore = __String::createWithFormat("SCORE : %i", score);
	Label *scoreLabel = Label::createWithTTF(tempscore->getCString(),
			"fonts/burnstown dam.ttf", h * SCORE_FONT_SIZE);
	//scoreLabel->setColor(Color3B::WHITE);
	Size s1 = scoreLabel->getBoundingBox().size;
	scoreLabel->setPosition(s1.width / 1.5f, s1.height/1.5f);
	this->addChild(scoreLabel, 100);

	auto highscore = def->getIntegerForKey("HIGHSCORE");
	__String *tempscore2 = __String::createWithFormat("BEST : %i", highscore);
	Label *bestScoreLabel = Label::createWithTTF(tempscore2->getCString(),
			"fonts/burnstown dam.ttf", h * SCORE_FONT_SIZE);
	bestScoreLabel->setColor(Color3B(92,74,39));
	Size s2 = bestScoreLabel->getBoundingBox().size;
	bestScoreLabel->setPosition(w - s2.width / 1.5f, s2.height/1.5f);
	this->addChild(bestScoreLabel, 100);
	this->scheduleOnce(schedule_selector(GameOverScene::sound),0.05f);
	this->addChild(menu, 1);
	return true;
}
void GameOverScene::sound(float dt){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/gameover.mp3");
}
void GameOverScene::Replay(cocos2d::Ref * sender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button.mp3");
	Scene *scene = GameScene::createScene();
	Director::getInstance()->replaceScene(
			TransitionFade::create(TRANSACTION_TIME, scene));

}
void GameOverScene::ReturnToMainMenu(cocos2d::Ref * sender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button.mp3");
	Director::getInstance()->popScene();

}

