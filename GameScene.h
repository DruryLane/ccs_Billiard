#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "GameManager.h"
#include "Box2D/Box2D.h"

class GameScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

	void doMsgReceived(Ref* obj);
	void tick(float dt);
	bool createBox2dWorld(bool debug);
	void initBackGround();
	~GameScene();
	b2Body* getBodyAtTab(Vec2 p);
	b2Body* addNewSprite(Vec2 point, Color3B color);
	b2Body* addNewSprite(Vec2 point);

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	Size winSize;
	b2World* _world;

	b2Vec2 startPoint;
	b2Vec2 endPoint;
	b2Body* myBall;
	bool bBallTouch;
};

#endif // __GAME_SCENE_H__
