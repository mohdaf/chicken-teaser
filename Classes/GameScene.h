#ifndef __GAME__SCENE_H__
#define __GAME__SCENE_H__

#include "cocos2d.h"

class GameScene: public cocos2d::Layer {
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	cocos2d::Sprite *mysprite,*bomb;

	cocos2d::Label *scoreLabel;

	cocos2d::MenuItemImage *PauseItem;
	cocos2d::MenuItemImage *ResumeItem;
	cocos2d::MenuItemImage *RestartItem;
	cocos2d::MenuItemImage *ReturntomainItem;



	int w;
	int h;
	float diff,speed;
	int maxchicks;
	unsigned int score,toDie,boom_sid,fuse_sid,rush_count;
	cocos2d::Sprite *egg;
	float egg_h;
	float egg_w;
	float mw;
	cocos2d::Array *curreggs;
	cocos2d::Array *downeggs;
	cocos2d::Sprite *souls[4];
	cocos2d::Sprite *rushs[4];

	short large;

	bool dead,gold,rush;

	float *xs, *ys;
	float sx, sy;
	float timebetweeneggs;
	float timeelapsed;
	float timebetweeneggs_rush;
	float timeelapsed_rush;
	float timetostop;
	int index;
	void DisableRush(float dt);
	void checker(float dt);
	void GamePlay(float dt);
	float *Tbitween;
	int getRandomNumber(int from, int to);

	void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *e);
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	void remove(float dt);
	void GoToGameOver(float dt);
	void GoToGameOver2(cocos2d::Ref *sender);
	void ReturnSpeed(float dt);
	void ReturnScale1(float dt);
	void ReturnScale2(float dt);
	void Pause(cocos2d::Ref *sender);
	void Resume(cocos2d::Ref *sender);
	void Restart(cocos2d::Ref *sender);
	void ReturnToMainMenu(cocos2d::Ref * sender);
};

#endif // __GAME__SCENE_H__
