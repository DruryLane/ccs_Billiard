#ifndef __BILLIARD_BALL_H__
#define __BILLIARD_BALL_H__

#include "GameManager.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class BilliardBall {
public:
	BilliardBall(Color3B color);
	
	Sprite* getSprite();
	b2Body* getBody();
	Vec2 getPosition();
	
	void setSprite(Sprite* sprite);
	void setBody(b2Body* body);
private:
	Sprite* pSprite;
	b2Body* pBody;
};

#endif // __BILLIARD_BALL_H__