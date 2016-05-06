#ifndef __BILLIARD_BALL_H__
#define __BILLIARD_BALL_H__

#define FRICTION1	4.0f	//운동마찰력계수

#include "GameManager.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class BilliardBall {
public:
	BilliardBall(Color3B color, int num);

	void initAngularVelocity(Vec2 targetPosition);
	void updateBilliardBall(float dt);

	void updateSprite();
	void updateLinearVelocity(float dt);
	
	Sprite* getSprite();
	b2Body* getBody();
	Vec2 getPosition();
	int getBallNum();
	bool isTarget();
	
	void setSprite(Sprite* sprite);
	void setBody(b2Body* body);
	void setTarget(bool t);
	void setBallNum(int num);
	void setAngularVelocity(Vec3 m_angularVelocity);

private:
	Sprite* pSprite;
	b2Body* pBody;
	bool target;
	int ballNum;
	float32 mass;
	Vec3 angularVelocity;
};

#endif // __BILLIARD_BALL_H__