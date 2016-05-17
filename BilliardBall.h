#ifndef __BILLIARD_BALL_H__
#define __BILLIARD_BALL_H__

#define FRICTION_SLIDE		2.0f	//운동저항
#define FRICTION_ROLLING	12.0f	//회전저항
#define ANGULAR_POWER		0.005f

#include "GameManager.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class BilliardBall {
public:
	BilliardBall(Color3B color, int num);

	void initAngularVelocity(Vec2 targetPosition);
	void initSprite(Color3B color);
	
	void updateBilliardBall(float dt);
	void updateSprite();
	void updateLinearVelocity(float dt);
	void updateAngualrVelocity(float dt);
	
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

	b2Vec2 angularToLinear(Vec3 m_angularVelocity);

private:
	Sprite* pSprite;
	b2Body* pBody;
	bool target;
	int ballNum;
	float32 mass;
	Vec3 angularVelocity;
	b2Vec2 linearA;
};

#endif // __BILLIARD_BALL_H__