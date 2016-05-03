#ifndef __BILLIARD_BALL_H__
#define __BILLIARD_BALL_H__

#include "GameManager.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class BilliardBall {
public:
	BilliardBall(Color3B color, int num);
	
	Sprite* getSprite();
	b2Body* getBody();
	Vec2 getPosition();
	int getBallNum();
	bool isTarget();
	
	void setSprite(Sprite* sprite);
	void setBody(b2Body* body);
	void setTarget(bool t);
	void setBallNum(int num);
private:
	Sprite* pSprite;
	b2Body* pBody;
	bool target;
	int ballNum;
};

#endif // __BILLIARD_BALL_H__