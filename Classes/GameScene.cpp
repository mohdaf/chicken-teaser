#include "GameScene.h"
#include "GameOverScene.h"
#include "GameOverScene.cpp"
#include "Definitions.h"
#include "AdmobHelper.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init() {

	if (!Layer::init()) {
		return false;
	}
	//Sound Effects preloading
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("sounds/bomb fall.mp3");
	audio->preloadEffect("sounds/bomb falling.mp3");
	audio->preloadEffect("sounds/bomb fuse.mp3");
	audio->preloadEffect("sounds/boom.mp3");
	audio->preloadEffect("sounds/Chick.mp3");
	audio->preloadEffect("sounds/egg catch.mp3");
	audio->preloadEffect("sounds/egg smash.mp3");
	audio->preloadEffect("sounds/poop fall.mp3");
	audio->preloadEffect("sounds/golden egg catch.mp3");
	audio->preloadEffect("sounds/button.mp3");
	audio->preloadEffect("sounds/rush.mp3");
	audio->preloadEffect("sounds/enlarge.mp3");
	audio->preloadEffect("sounds/dislarge.mp3");
	audio->preloadEffect("sounds/poop-eat.mp3");
	audio->preloadEffect("sounds/purple.mp3");
	audio->preloadEffect("sounds/fast.mp3");
	audio->preloadEffect("sounds/slow.mp3");
	audio->preloadEffect("sound/poop.mp3");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Screen Dimensions
	w = visibleSize.width;
	h = visibleSize.height;

	dead = gold = rush = false;
	index = rush_count = 0;
	timeelapsed = timeelapsed_rush = timetostop = 0;
	timebetweeneggs = 3.0f;
	timebetweeneggs_rush = 0.1f;
	toDie = 3;

	large = 0;
	// Initializing difficulty and speed
	diff = 1.0f;
	speed = DEFAULT_SPPED;
	// Number of hens
	maxchicks = 5;
	curreggs = downeggs = Array::create();
	curreggs->retain();
	downeggs->retain();
	egg = Sprite::create("images/egg-score.png");
	egg_h = egg->getContentSize().height;
	egg_w = egg->getContentSize().width;
	// BackGround
	Sprite * bgsprite = Sprite::create("images/background.png");
	bgsprite->setPosition(Vec2(w / 2, h / 2));
	bgsprite->setScale(w / bgsprite->getContentSize().width,
			h / bgsprite->getContentSize().height);
	this->addChild(bgsprite, -4);
	//Scoring
	score = 0;
	__String *tempscore = __String::createWithFormat("%i", score);
	scoreLabel = Label::createWithTTF(tempscore->getCString(),
			"fonts/Marker Felt.ttf", h * SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B(184, 154, 74));
	scoreLabel->setPosition(scoreLabel->getContentSize().width * 2,
			h - scoreLabel->getContentSize().height);
	this->addChild(scoreLabel, 100);
	//souls
	for (short var = 0; var < 4; ++var) {
		Sprite *soul = Sprite::create("images/egg-score.png");
		soul->setScale((w / soul->getContentSize().width) * .05f);
		Size s = soul->getBoundingBox().size;
		soul->setPosition((var * (s.width + 10)) + s.width / 2, s.height / 2);
		souls[var] = soul;
		this->addChild(soul, 0);
	}
	for (short var = 0; var < 4; ++var) {
		Sprite *elem = Sprite::create("images/purple-egg.png");
		elem->setScale((w / elem->getContentSize().width) * .05f);
		Size s = elem->getBoundingBox().size;
		elem->setPosition(((var + 4) * (s.width + 10)) + s.width / 2,
				s.height / 2);
		rushs[var] = elem;
		elem->setVisible(false);
		this->addChild(elem, 0);
	}

	xs = new float[5];
	ys = new float[5];
	for (int i = 1; i <= 5; i++) {
		Sprite * c = Sprite::create("images/chicken-rope.png");
		float scale = (w / c->getContentSize().width) * .13f;
		c->setScale(scale);
		auto x = (w / 6) * i;
		auto y = h - h / 30;
		c->setPosition(x, y);
		this->addChild(c);
		xs[i - 1] = x;
		ys[i - 1] = y - c->getBoundingBox().size.height / 2;
	}
	bomb = Sprite::create("images/boom.png");
	bomb->setScale((w / bomb->getContentSize().width) * 0.35f);
	bomb->setPosition(w / 2, h / 2);
	bomb->setVisible(false);
	this->addChild(bomb, 1);

	// creating an setting position of the collector then add it to the Layer
	mysprite = Sprite::create("images/egg-car.png");
	float scale = (w / mysprite->getContentSize().width) * 0.11f;
	mysprite->setPosition(Point((w / 2), (w / 10)));
	mysprite->setScale(scale);

	this->addChild(mysprite, -1);

	//pause button
	PauseItem = MenuItemImage::create("images/pause.png", "images/pause2.png",
			CC_CALLBACK_1(GameScene::Pause,this));
	float scale1 = (w / PauseItem->getContentSize().width) * 0.14f;
	PauseItem->setScale(scale1);
	Size s3 = PauseItem->getBoundingBox().size;
	PauseItem->setPosition(w - s3.width / 2.1f, s3.height / 1.5f);

	ResumeItem = MenuItemImage::create("images/resume.png",
			"images/resume2.png", CC_CALLBACK_1(GameScene::Resume,this));
	scale1 = (w / ResumeItem->getContentSize().width) * 0.2f;
	ResumeItem->setScale(scale1);
	Size s4 = ResumeItem->getBoundingBox().size;
	ResumeItem->setPosition((w / 2) - (s4.width / 1.8), h / 2);
	ResumeItem->setVisible(false);

	ReturntomainItem = MenuItemImage::create("images/resign.png",
			"images/resign2.png", CC_CALLBACK_1(GameScene::GoToGameOver2,this));
	scale1 = (w / ReturntomainItem->getContentSize().width) * 0.2f;
	ReturntomainItem->setScale(scale1);
	s4 = ReturntomainItem->getBoundingBox().size;
	ReturntomainItem->setPosition((w / 2) + (s4.width / 1.8), h / 2);
	ReturntomainItem->setVisible(false);

	RestartItem = MenuItemImage::create("images/restart.png",
			"images/restart2.png", CC_CALLBACK_1(GameScene::Restart,this));
	scale1 = (w / RestartItem->getContentSize().width) * 0.1f;
	RestartItem->setScale(scale1);
	Size s5 = RestartItem->getBoundingBox().size;
	RestartItem->setPosition(w - s5.width / 2.0f, h - s5.height / 1.5f);

	Menu *m = Menu::create(PauseItem, ResumeItem, RestartItem, ReturntomainItem,
			NULL);
	m->setPosition(0, 0);
	this->addChild(m, 1);

	//Listening For the Accelerometer
	Device::setAccelerometerEnabled(true);
	//Device::setAccelerometerInterval(100);
	auto listener = EventListenerAcceleration::create(
			CC_CALLBACK_2(GameScene::onAcceleration,this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//Scheduling the game play function function
	this->schedule(schedule_selector(GameScene::GamePlay), 1 / 60);

	mw = mysprite->getBoundingBox().size.width;

	return true;
}

bool inrange(int i, int l, int u) {
	return (i <= u) && (i >= l);
}

void GameScene::GamePlay(float dt) {
	if (!rush) {
		timeelapsed += dt;
		if (toDie == -1) {
			curreggs->removeAllObjects();
			downeggs->removeAllObjects();
			UserDefault *def = UserDefault::getInstance();
			auto highscore = def->getIntegerForKey("HIGHSCORE", 0);
			def->setIntegerForKey("SCORE", score);
			if (score > highscore) {
				def->setIntegerForKey("HIGHSCORE", score);
			}
			def->flush();
			GoToGameOver(0.5f);
			dead = true;
		}
		Ref *s;
		CCARRAY_FOREACH(curreggs,s) {
			Sprite * t = (Sprite *) s;
			float eggY = t->getPositionY()
					- (t->getBoundingBox().size.height / 2);
			float spriteY = mysprite->getPositionY()
					+ mysprite->getBoundingBox().size.height / 2;
			if (eggY > spriteY - h / 30)
				continue;
			else if (eggY < spriteY - h / 20) {
				downeggs->addObject(s);
				curreggs->removeObject(s, true);
				continue;
			} else if (t->getBoundingBox().intersectsRect(
					mysprite->getBoundingBox())) {
				int tag = t->getTag();
				if (tag == 1) {
					score++;
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/egg catch.mp3");
					__String *tempscore = __String::createWithFormat("%i",
							score);
					scoreLabel->setString(tempscore->getCString());
					timebetweeneggs -= 0.015f;
					diff += .005f;
					if (score % 10 == 0)
						gold = true;
				} else if (tag == 2) {
					souls[toDie]->setVisible(false);
					toDie--;
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/poop-eat.mp3");
				} else if (tag == 3) {
					CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(
							boom_sid);
					CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(
							fuse_sid);
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/boom.mp3");
					const Vec2& pos = mysprite->getPosition();
					bomb->setPosition(pos.x, pos.y);
					bomb->setVisible(true);
					mysprite->setVisible(false);
					Action *fade = FadeOut::create(3.0f);
					bomb->runAction(fade);
					Sprite * broken = Sprite::create("images/broken car.png");
					broken->setScale(
							(w / broken->getContentSize().width) * 0.18f);
					broken->setPosition(pos.x, pos.y - 10);
					this->addChild(broken, 0);

					UserDefault *def = UserDefault::getInstance();
					auto highscore = def->getIntegerForKey("HIGHSCORE", 0);
					def->setIntegerForKey("SCORE", score);
					if (score > highscore) {
						def->setIntegerForKey("HIGHSCORE", score);

					}
					def->flush();
					dead = true;
					this->scheduleOnce(
							schedule_selector(GameScene::GoToGameOver), 6.0f);
				} else if (tag == 4) {
					if (toDie < 3) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
								"sounds/golden egg catch.mp3");
						toDie++;
						souls[toDie]->setVisible(true);
					}
				} else if (tag == 5) {
					if (rush_count < 4) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
								"sounds/purple.mp3");
						rush_count++;
						rushs[rush_count - 1]->setVisible(true);
					} else {
						auto audio =
								CocosDenshion::SimpleAudioEngine::getInstance();
						audio->playEffect("sounds/golden egg catch.mp3");
						audio->stopBackgroundMusic(false);
						audio->playBackgroundMusic("sounds/rush.mp3", false);
						rush_count = 0;
						rushs[0]->setVisible(false);
						rushs[1]->setVisible(false);
						rushs[2]->setVisible(false);
						rushs[3]->setVisible(false);
						rush = true;
						this->scheduleOnce(
								schedule_selector(GameScene::DisableRush),
								13.5f);
					}

				} else if (tag == 6) {
					score++;
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/egg catch.mp3");
					__String *tempscore = __String::createWithFormat("%i",
							score);
					scoreLabel->setString(tempscore->getCString());
					if (score % 10 == 0)
						gold = true;
				} else if (tag == 7) {
					large = -1;
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/dislarge.mp3");
					Action *ac = ScaleBy::create(1.0, 0.5f, 1.0f);
					mysprite->runAction(ac);
					this->scheduleOnce(
							schedule_selector(GameScene::ReturnScale1), 8.0f);
				} else if (tag == 8) {
					large = 1;
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/enlarge.mp3");
					Action *ac = ScaleBy::create(1.0, 2.0f, 1.0f);
					mysprite->runAction(ac);
					this->scheduleOnce(
							schedule_selector(GameScene::ReturnScale2), 8.0f);
				} else if (tag == 9) {
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/slow.mp3");
					speed = 0.03f;
					this->scheduleOnce(
							schedule_selector(GameScene::ReturnSpeed), 6.0f);
				}
				curreggs->removeObject(s, true);
				this->removeChild(t, true);
			}
		}
		CCARRAY_FOREACH(downeggs,s) {
			Sprite * cur = (Sprite *) s;
			if (cur->getPositionY() <= h / 9) {
				this->removeChild(cur);
				Sprite * broken;
				int tag = cur->getTag();
				if (tag == 1) {
					souls[toDie]->setVisible(false);
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/egg smash.mp3");
					toDie--;
					broken = Sprite::create("images/broken-egg.png");
				} else if (tag == 2) {
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/poop.mp3");
					broken = Sprite::create("images/poop.png");
				} else if (tag == 3) {
					CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(
							boom_sid);
					CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(
							fuse_sid);
					broken = cur;
					cur->stopAllActions();
				} else if (tag == 4) {
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/egg smash.mp3");
					broken = Sprite::create("images/broken-egg-golden.png");
				} else if (tag == 5) {
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/egg smash.mp3");
					broken = Sprite::create("images/broken-egg-purple.png");

				} else if (tag == 6 || tag == 7 || tag == 8 || tag == 9) {
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"sounds/egg smash.mp3");
					broken = Sprite::create("images/broken-egg-silver.png");
				}
				broken->setPosition(cur->getPositionX(), cur->getPositionY());
				float scale = (w / broken->getContentSize().width) * .05f;
				broken->setScale(scale, scale);
				this->addChild(broken, -2);
				auto act = FadeOut::create(3);
				auto act2 = MoveBy::create(6, Vec2(0, 10));
				broken->runAction(act);
				broken->runAction(act2);
				downeggs->removeObject(s, true);
				this->scheduleOnce(schedule_selector(GameScene::remove), 3.0f);
			}
		}

		//throwing eggs
		if (timeelapsed >= timebetweeneggs && !dead) {
			int pos = rand() % (maxchicks);
			int type = rand() % 100;
			if (gold) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/poop fall.mp3");
				Sprite *news = Sprite::create("images/golden-egg.png");
				news->setTag(4);
				float scale = (w / news->getContentSize().width) * .03f;
				news->setScale(scale);
				news->setPosition(xs[pos], ys[pos]);
				this->addChild(news, -2);
				curreggs->addObject(news);
				auto act = MoveBy::create(5 - diff, Point(0, -w));
				news->runAction(act);
				gold = false;
			} else if (type > 55) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/Chick.mp3");
				Sprite *news = Sprite::create("images/egg-score.png");
				news->setTag(1);
				float scale = (w / news->getContentSize().width) * .03f;
				news->setScale(scale);
				news->setPosition(xs[pos], ys[pos]);
				this->addChild(news, -2);
				curreggs->addObject(news);
				Action *act = MoveBy::create(5 - diff, Point(0, -w));
				news->runAction(act);
			} else if (inrange(type, 1, 10)) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/bomb fall.mp3");
				boom_sid =
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
								"sounds/bomb falling.mp3");
				fuse_sid =
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
								"sounds/bomb fuse.mp3");
				Sprite *news = Sprite::create("images/bomb.png");
				news->setTag(3);
				float scale = (w / news->getContentSize().width) * .06f;
				news->setScale(scale);
				news->setPosition(xs[pos], ys[pos]);
				this->addChild(news, -2);
				curreggs->addObject(news);
				Action *act = MoveBy::create(5 - diff, Point(0, -w));
				Action *act2 = RotateBy::create(5 - diff, 720);
				news->runAction(act);
				news->runAction(act2);
			} else if (inrange(type, 11, 20)) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/poop fall.mp3");
				Sprite *news = Sprite::create("images/falling_poop.png");
				news->setTag(2);
				float scale = (w / news->getContentSize().width) * .06f;
				news->setScale(scale);
				news->setPosition(xs[pos], ys[pos]);
				this->addChild(news, -2);
				curreggs->addObject(news);
				Action *act = MoveBy::create(5 - diff, Point(0, -w));
				news->runAction(act);
			} else if (inrange(type, 21, 35)) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/poop fall.mp3");
				Sprite *news = Sprite::create("images/purple-egg.png");
				news->setTag(5);
				float scale = (w / news->getContentSize().width) * .03f;
				news->setScale(scale);
				news->setPosition(xs[pos], ys[pos]);
				this->addChild(news, -2);
				curreggs->addObject(news);
				Action *act = MoveBy::create(5 - diff, Point(0, -w));
				news->runAction(act);

			} else {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/poop fall.mp3");
				Sprite *news = Sprite::create("images/silver-egg.png");
				int t = rand() % 100;
				//gold
				if (inrange(t, 0, 9))
					news->setTag(4);
				//enlarge
				else if (inrange(t, 10, 29) && large == 0)
					news->setTag(7);
				//dislarg
				else if (inrange(t, 30, 69) && large == 0)
					news->setTag(8);
				//slow
				else if (inrange(t, 70, 89))
					news->setTag(9);
				//else
				else
					news->setTag(6);
				float scale = (w / news->getContentSize().width) * .03f;
				news->setScale(scale);
				news->setPosition(xs[pos], ys[pos]);
				this->addChild(news, -2);
				curreggs->addObject(news);
				Action *act = MoveBy::create(5 - diff, Point(0, -w));
				news->runAction(act);

			}
			timeelapsed = 0;
		}
	} else {
		//here rush mode
		timeelapsed_rush += dt;
		timetostop += dt;
		if (timeelapsed_rush >= timebetweeneggs_rush && timetostop < 10
				&& !dead) {
			int pos = rand() % (maxchicks);
			Sprite *news = Sprite::create("images/egg-score.png");
			news->setTag(10);
			float scale = (w / news->getContentSize().width) * .03f;
			news->setScale(scale);
			news->setPosition(xs[pos], ys[pos]);
			this->addChild(news, -2);
			curreggs->addObject(news);
			Action *act = MoveBy::create(5 - diff, Point(0, -w));
			news->runAction(act);
			timeelapsed_rush = 0;
		}
		Ref *s;
		CCARRAY_FOREACH(curreggs,s) {
			Sprite * t = (Sprite *) s;
			float eggY = t->getPositionY()
					- (t->getBoundingBox().size.height / 2);
			float spriteY = mysprite->getPositionY()
					+ mysprite->getBoundingBox().size.height / 2;
			if (eggY > spriteY - h / 30)
				continue;
			else if (eggY < spriteY - h / 20) {
				curreggs->removeObject(s, true);
				downeggs->addObject(s);
				continue;
			} else if (t->getBoundingBox().intersectsRect(
					mysprite->getBoundingBox())) {
				score++;
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"sounds/egg catch.mp3");
				__String *tempscore = __String::createWithFormat("%i", score);
				scoreLabel->setString(tempscore->getCString());
				curreggs->removeObject(s, true);
				this->removeChild(t, true);
			}

		}
		CCARRAY_FOREACH(downeggs,s) {
			Sprite * cur = (Sprite *) s;
			if (cur->getPositionY() <= -50) {
				this->removeChild(cur, true);
				downeggs->removeObject(s, true);
			}
		}
	}
}
void GameScene::DisableRush(float dt) {
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
			"sounds/game.mp3", true);
	rush = false;
	timetostop = 0;
}
void GameScene::remove(float dt) {
	this->removeChild((Sprite *) downeggs->getObjectAtIndex(0), true);
	CCLog("cleand");
}
void GameScene::onAcceleration(Acceleration *acc, Event *e) {
	int sy = mysprite->getPositionY();
	float addX = acc->x * w * speed;
	int newX = mysprite->getPositionX() + addX;

	if (newX > w - mw / 2) {
		newX = w;
		mysprite->setPosition(w - mw / 2, sy);
		return;
	}
	if (newX < 0 + mw / 2) {
		newX = 0;
		mysprite->setPosition(0 + mw / 2, sy);
		return;
	}
	auto action = MoveTo::create(.15, Point(newX, sy));
	mysprite->runAction(action);
}
void GameScene::ReturnSpeed(float dt) {
	if (!dead) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				"sounds/fast.mp3");
		speed = DEFAULT_SPPED;
	}
}
void GameScene::ReturnScale1(float dt) {
	if (!dead) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				"sounds/enlarge.mp3");
		Action *ac = ScaleBy::create(1.0, 2.0f, 1.0f);
		mysprite->runAction(ac);
		large = 0;
	}

}
void GameScene::ReturnScale2(float dt) {
	if (!dead) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				"sounds/dislarge.mp3");
		Action *ac = ScaleBy::create(1.0, 0.5f, 1.0f);
		mysprite->runAction(ac);
		large = 0;
	}
}
void GameScene::GoToGameOver(float dt) {
	auto over = GameOverScene::createScene();
	Director::getInstance()->replaceScene(
			TransitionFade::create(TRANSACTION_TIME, over));

}
void GameScene::Pause(Ref *sender) {
	if (!dead) {
		Device::setAccelerometerEnabled(false);
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				"sounds/button.mp3");
		PauseItem->setVisible(false);
		ResumeItem->setVisible(true);
		ReturntomainItem->setVisible(true);
		Director::getInstance()->pause();
	}
}
void GameScene::Resume(Ref *sender) {
	Device::setAccelerometerEnabled(true);
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"sounds/button.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	ResumeItem->setVisible(false);
	ReturntomainItem->setVisible(false);
	PauseItem->setVisible(true);
	Director::getInstance()->resume();
}
void GameScene::Restart(Ref *sender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	Device::setAccelerometerEnabled(true);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"sounds/button.mp3");
	if (Director::getInstance()->isPaused()) {
		Director::getInstance()->resume();
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	Scene *re = GameScene::createScene();
	Director::getInstance()->replaceScene(
			TransitionFade::create(TRANSACTION_TIME, re));
}
void GameScene::GoToGameOver2(Ref *sender) {
	Director::getInstance()->resume();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
				"sounds/button.mp3");
	curreggs->removeAllObjects();
	downeggs->removeAllObjects();
	UserDefault *def = UserDefault::getInstance();
	auto highscore = def->getIntegerForKey("HIGHSCORE", 0);
	def->setIntegerForKey("SCORE", score);
	if (score > highscore) {
		def->setIntegerForKey("HIGHSCORE", score);
	}
	def->flush();
	GoToGameOver(0.5f);
	dead = true;

}
