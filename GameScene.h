#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "GameManager.h"

class GameScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

	void doMsgReceived(Ref* obj);
};

#endif // __GAME_SCENE_H__
