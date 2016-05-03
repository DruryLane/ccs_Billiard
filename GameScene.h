#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "GameManager.h"
#include "Box2D/Box2D.h"
#include "BilliardBall.h"
#include "ContactListener.h"

class GameScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

	void doMsgReceived(Ref* obj);
	void tick(float dt);
	bool createBox2dWorld();
		void initBox2dWorld(b2Vec2 g);
		void initBall();
			b2Body* createBall(Vec2 position, BilliardBall* pBilliardBall);
	void initBackGround();
	void initCue();

	void turnStart();
	void turnEnd();
	
	~GameScene();

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	Size winSize;
	b2World* _world;
	ContactListener* myContactLstener;

	BilliardBall* playerBall[2];
	BilliardBall* otherBall1;
	BilliardBall* otherBall2;
	Sprite* pCue;
	Sprite* pCueBox;
	Sprite* pCuePower;
	Vec2 force;
	bool isPhysical;
	bool turn;

	int curTurn;
	float power;
	float gRotation;
	bool bSelect;
};

#endif // __GAME_SCENE_H__
