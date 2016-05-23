#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "GameManager.h"

class IntroScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(IntroScene);

	virtual void onEnter();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	Size winSize;
};

#endif // __INTRO_SCENE_H__
