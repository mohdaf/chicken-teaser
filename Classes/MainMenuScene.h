#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);

    cocos2d::Label *bestScoreLabel;

    void GoToGameScene(cocos2d::Ref *sender);
    void start(float dt);
    void Exit(cocos2d::Ref *sender);
    void quite(float dt);
    void Multi_player(cocos2d::Ref *sender);

};

#endif // __MAIN_MENU_SCENE_H__
